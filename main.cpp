/**
  * Nötige Pakete
  *
  * - libqt4-core
  * - libqt4-sql
  * - libqt4-sql-psql
  * - libldap2-dev
  */
#include <unistd.h>

#include <QtCore>
#include <TcpServer.h>
#include <QTcpSocket>
#include <QSettings>
#include <QMultiMap>
#include "Config.h"

#include <Logger/Logger.h>
#include <Logger/LoggerConsoleAppender.h>
#include <Logger/LoggerFileAppender.h>
#include <TcpCommandFactories.h>

#include "BackupMonitorDoCacheThread.h"

#include <TcpCommands/main/serverVersionFactory.h>
#include <TcpCommands/main/userLoginFactory.h>
#include <TcpCommands/main/connectTcpFactory.h>

#include <TcpCommands/loggedInUser/getUserColumnFactory.h>
#include "TcpCommands/loggedInUser/hasTcpPermissionFactory.h"

#include <TcpCommands/system/getAllTcpCommandsFactory.h>
#include <TcpCommands/system/getAllSystemGroupsFactory.h>
#include <TcpCommands/system/saveTcpCommandsPermissionsFactory.h>
#include <TcpCommands/system/serverStatisticsFactory.h>

#include <TcpCommands/administration/testLDAPGroupMembershipsFactory.h>
#include <TcpCommands/administration/userGroupModFactory.h>
#include <TcpCommands/administration/getAllUserGroupsFactory.h>
#include <TcpCommands/administration/getUserGroupFactory.h>
#include <TcpCommands/administration/userGroupToSystemGroupsFactory.h>
#include <TcpCommands/administration/userGroupDeleteFactory.h>

#include <TcpCommands/backupMonitor/getCustomersFactory.h>
#include <TcpCommands/backupMonitor/getJobsFactory.h>
#include <TcpCommands/backupMonitor/getJobFactory.h>
#include <TcpCommands/backupMonitor/getJobProtocolFactory.h>
#include <TcpCommands/backupMonitor/getBackupStateOfDayFactory.h>
#include <TcpCommands/backupMonitor/getCustomerGroupsFactory.h>
#include <TcpCommands/backupMonitor/customerGroupModFactory.h>
#include <TcpCommands/backupMonitor/customerGroupDeleteFactory.h>
#include <TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.h>
#include <TcpCommands/backupMonitor/getLogFactory.h>
#include <TcpCommands/backupMonitor/createLogFactory.h>

#include <TcpCommands/customers/searchFactory.h>
#include <TcpCommands/customers/getCustomerFactory.h>
#include <TcpCommands/customers/modCustomerFactory.h>
#include <TcpCommands/customers/generateNewBackupMonitorKeyFactory.h>
#include <TcpCommands/customers/customerEditableFactory.h>
#include <TcpCommands/customers/removeCustomerFactory.h>

#include <TcpCommands/notifications/getLastMessagesFactory.h>
#include <TcpCommands/notifications/createFactory.h>

#include <Database/DatabasePool.h>

#include <dbModels/dbTcpActionModel.h>
#include <dbModels/dbSystemGroupsModel.h>

#include "Registry.h"

// The main function :) ...
int main(int argc, char **argv) {

    QCoreApplication app(argc, argv);

    QStringList programArguments = app.arguments();

    // Wir müssen Englische Sprache setzen, sonst haben wir Probleme mit Datumsausgaben & SQL Queries!!!
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    Config* c = Config::instance();


    /**
      * File Logging konfigurieren:
      */
    Logger* logger = Logger::instance();

    //logger->addAppender( new LoggerConsoleAppender(logger) );
    logger->addAppender( new LoggerFileAppender(c->get("logging", "path").toUtf8(), logger) );

    logger->setLogLevel(static_cast<Logger::Categories>( c->get("logging", "level", "4").toInt()));

    logNotice("==========================================");
    logNotice("         INTRANET SERVER");
    logNotice("        SIEDL NETWORKS GMBH");
    logNotice("==========================================");

    // Wenn Parameter "--fullTcpPermissions" gesetzt wurde, so hat der durch den Parameter angegebene Benutzer alle TCP Rechte...
    {
        Registry::set("fullTcpPermissions", QVariant( false ) );

        int pos = programArguments.indexOf("--fullTcpPermissions");
        if (pos != -1 && programArguments.size() - 1 > pos) {
            QString username = app.arguments().at(pos + 1);
            if (username.isEmpty()) {
                logFatal("argument syntax error: --fullTcpPermissions [username]!");
                return 100;
            }
            else {
                Registry::set("fullTcpPermissions", QVariant( true ) );
                Registry::set("fullTcpPermissionsUsername", username );
            }
        }
        else if (programArguments.size() - 1 == pos) {
            logFatal("argument syntax error: --fullTcpPermissions [username]!");
            return 100;
        }
    }

    /**
      * Datenbankverbindungen initialisieen:
      */

    bool poolInitSuccessful = false;
    while (!poolInitSuccessful) {
        try {
            DatabasePool::init(c->get("server", "database_min_connections").toInt(),
                               c->get("server", "database_max_connections").toInt(),
                               c->get("database", "hostname"),
                               c->get("database", "username"),
                               c->get("database", "password"),
                               c->get("database", "database")
                               );

            poolInitSuccessful = true;
        }
        catch (SqlException &e) {

            logFatal("Ein Fehler beim Verbinden zur Datenbank ist aufgetreten!");
            logFatal("SQL: \n%1\n%2", e.sql(), e.error().databaseText());
            logFatal("Driver error: %1", e.error().driverText() );

            logFatal("Try again in 10 seconds...");
            sleep(10);
        }
    }

    // Nun ermitteln wir die ID der Systemgruppe "LoggedOff"
    try {
        /**
          * @TODO: Wir nötigen eine Registry!
          */
        int loggedOffId = dbSystemGroups::getIdByName("LoggedOff");
        if (loggedOffId == -1) {
            logFatal("Could not get the ID auf system group 'LoggedOff'. System group not found!");

            return 100;
        }
    }
    catch (SqlException &e) {

        logFatal("Ein SQL Fehler ist aufgetreten!");
        logFatal("SQL: \n%1\n%2", e.sql(), e.error().databaseText());
        logFatal("Driver error: %1", e.error().driverText() );

        return 100;
    }

    /**
      * Alle Aktionen des Tcp- Servers registrieren:
      *
      * @TODO: Wenn Factory bereits existiert, so muss der Speicher wieder freigegeben werden.
      */
    TcpCommandFactories *tcpFacts = TcpCommandFactories::instance();
    tcpFacts->registerFactory( new TcpCommandFactory_main_userLogin );
    tcpFacts->registerFactory( new TcpCommandFactory_main_serverVersion );
    tcpFacts->registerFactory( new TcpCommandFactory_main_connectTcp );
    tcpFacts->registerFactory( new TcpCommandFactory_loggedInUser_getUserColumn );
    tcpFacts->registerFactory( new TcpCommandFactory_loggedInUser_hasTcpPermission );
    tcpFacts->registerFactory( new TcpCommandFactory_system_getAllTcpCommands );
    tcpFacts->registerFactory( new TcpCommandFactory_system_getAllSystemGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_system_saveTcpCommandsPermissions );
    tcpFacts->registerFactory( new TcpCommandFactory_system_serverStatistics );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_testLDAPGroupMemberships );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_userGroupMod );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_getAllUserGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_getUserGroup );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_userGroupToSystemGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_userGroupDelete );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getCustomers );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_search );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_getCustomer );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_modCustomer );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_generateNewBackupMonitorKey );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_customerEditable );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_removeCustomer );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getJobs );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getJob );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getJobProtocol );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getBackupStateOfDay );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_customerGroupMod );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getCustomerGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_customerGroupDelete );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress );
    tcpFacts->registerFactory( new TcpCommandFactory_notifications_getLastMessages );
    tcpFacts->registerFactory( new TcpCommandFactory_notifications_create );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_createLog );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getLog );

    /**
      * Nun legen wir alle noch nicht existierenden Factories in der Datenbank an zur Rechteverwaltung
      */
    QMultiHash<QByteArray, QByteArray> factoryList = tcpFacts->getFactories();
    for (QMultiHash<QByteArray, QByteArray>::iterator it = factoryList.begin();
         it != factoryList.end(); it++) {

        try {
            if (dbTcpAction::createIfNotExists(it.key(), it.value())) {
                logDebug("created tcp action %1/%2 into databse...", it.key(), it.value());
            }
            else {
                logDebug("tcp action %1/%2 already exists... continue...", it.key(), it.value());
            }
        }
        catch (SqlException &e) {

            logFatal("Ein SQL Fehler ist aufgetreten!");
            logFatal("SQL: \n%1\n%2", e.sql(), e.error().databaseText());
            logFatal("Driver error: %1", e.error().driverText() );

            return 100;
        }
        catch (dbModelException &e) {

            logFatal("creating tcp action %1/%2 into database failed: %3", it.key(), it.value(), e.getMessage());

            return 100;
        }
    }

    /**
      * Tcp- Server erstellen:
      */
    logNotice("Try to create tcp socket...");
    TcpServer tcpServer;

    QHostAddress TCP_HOST = QHostAddress( Config::instance()->get("server", "listen_address") );
    quint16 TCP_PORT = Config::instance()->get("server", "listen_port", "4500").toInt();
    if (!tcpServer.listen(TCP_HOST, TCP_PORT)) {

        logFatal( QString("Could not creaste a socket on host '%1', port '%2'. Error message: %3")
                .arg(TCP_HOST.toString(), QString::number(TCP_PORT), tcpServer.errorString()) );

        return 100;
    }

    logNotice( QString("Listen on host '%1', port '%2'.").arg(TCP_HOST.toString(), QString::number(TCP_PORT)));
    logNotice("Waiting for incoming connections...");

    // Thread starten, welcher für das automatische Cachen zuständig ist:
    BackupMonitorDoCacheThread *cacheThread = new BackupMonitorDoCacheThread;
    cacheThread->start();

    // Nun lassen wir den Server seinen Spaß :)...
    return app.exec();
}
