/**
  * Führt eine User- Authentifizierung durch.
  *
  * @incoming-stream:
  *     @ TcpCommands::userLogin:   struct mit den Authentifizierungsdaten
  *
  * @outcoming-stream:
  *     @ TcpCommands::userLoginResponse:   struct mit der Antwort des Servers.
  */

#include <TcpCommands/main/userLogin.h>

#include "Logger/Logger.h"
#include "MyLdap/MyLdap.h"

#include "dbModels/dbUserModel.h"
#include "dbModels/dbUserGroupsModel.h"
#include "Config.h"

#include "Utils/UtilDataStream.h"
#include "Utils/UtilUsers.h"

TcpCommand_main_userLogin::TcpCommand_main_userLogin() : TcpAbstractCommand()
{

}

QVariant TcpCommand_main_userLogin::exec() {

    // Benutzerdaten ermitteln:
    TcpCommandsHelper::userLogin login;
    _streamRead(login);

    logNotice( QString("User authentification with username '%1'").arg(login.username) );

    Config *config = Config::instance();

    MyLdap ldap;
    ldap.connect( config->get("auth_backend_ldap", "address") );

    /**
      * Wir suchen im LDAP nach der DN des Benutzers...
      * Diese benötigen wir für die Benutzer- Authentifizierung
      */
    logNotice("Search user-account '%1' in the ldap database", login.username);
    QSharedPointer<MyLdapResult> searchResult =
            ldap.search( config->get("auth_backend_ldap", "user_search_base"), LDAP_SEARCH_SUB,
                         QString("("+ config->get("auth_backend_ldap", "attribute_username") +"=%1)").arg( ldap.escapeSearchFilter(login.username) ) );

    if (searchResult == 0) {
        return false;
    }

    if (searchResult->countEntries() != 1) {
        logError("The user- account does not exists or is not unique! Found %1 entries in the ldap!",
                 QString::number( searchResult->countEntries() ) );
        return false;
    }
    logDebug("Found 1 user- entry in the ldap...");
    QString userDn = searchResult->fetch()->getDn();
    logDebug("Found user entry in the ldap, dn is '%1'", userDn);

    /**
      * Und nun authentifizieren wir uns am LDAP Server mit der DN des Suchergebnisses:
      */
    if (! ldap.bind( userDn, login.password ) ) {
        logNotice("Authentification with username '%1' failed!", login.username);
        return false;
    }

    logNotice("Authentification with username '%1' successful!", login.username);

    // Benutzerdaten vom LDAP auslesen:
    searchResult = ldap.search( QString( config->get("auth_backend_ldap", "user_search_base") ), LDAP_SEARCH_SUB,
                                QString("("+ config->get("auth_backend_ldap", "attribute_username") +"=%1)").arg( ldap.escapeSearchFilter(login.username) ));

    if (searchResult == 0) {
        return false;
    }

    // Wir müssen nun genau einen Eintrag gefunden haben...
    if (searchResult->countEntries() != 1) {
        logError("The user- account ist not unique! Found %1 entries in the ldap!",
                 QString::number( searchResult->countEntries() ) );
        return false;
    }
    logDebug("Found 1 user- entry in the ldap...");

    QSharedPointer<MyLdapResultAttributes> userLdapAttributes = searchResult->fetch();

    // Parameter, welche wir vom LDAP benötigen:
    QSharedPointer< QList<QString> > list;

    /**
      * Alle Benutzerdaten vom LDAP- Baum auslesen...
      */
    list = userLdapAttributes->getValues( config->get("auth_backend_ldap", "attribute_username").toUtf8().data() );
    if (list->isEmpty() || list->first().isEmpty()) {
        logError("LDAP userdata not complete! Username- attribute '%1' is missing!",
                 config->get("auth_backend_ldap", "attribute_username") );
        return false;
    }
    QString username = list->first();
    logDebug("LDAP userdata: username is '%1'", username);

    // LdapID vom LDAP auslesen:
    list = userLdapAttributes->getValues( config->get("auth_backend_ldap", "attribute_unique_id").toUtf8().data() );
    if (list->isEmpty() || list->first().isEmpty()) {
        logError("LDAP userdata not complete! LdapID- attribute '%1' is missing!",
                 config->get("auth_backend_ldap", "attribute_unique_id") );
        return false;
    }
    QString ldapId = list->first();
    logDebug("LDAP userdata: ldapId is '%1'", ldapId);

    // Vornamen vom LDAP auslesen:
    list = userLdapAttributes->getValues( config->get("auth_backend_ldap", "attribute_givenname").toUtf8().data() );
    if (list->isEmpty() || list->first().isEmpty()) {
        logError("LDAP userdata not complete! GivenName- attribute '%1' is missing!",
                 config->get("auth_backend_ldap", "attribute_givenname") );
        return false;
    }
    QString givenName = list->first();
    logDebug("LDAP userdata: givenName is '%1'", givenName);

    // Nachnamen vom LDAP auslesen:
    list = userLdapAttributes->getValues( config->get("auth_backend_ldap", "attribute_surname").toUtf8().data() );
    if (list->isEmpty() || list->first().isEmpty()) {
        logError("LDAP userdata not complete! SurName- attribute '%1' is missing!",
                 config->get("auth_backend_ldap", "attribute_surname") );
        return false;
    }
    QString surName = list->first();
    logDebug("LDAP userdata: surName is '%1'", surName);

    // DisplayName vom LDAP auslesen:
    list = userLdapAttributes->getValues( config->get("auth_backend_ldap", "attribute_displayname").toUtf8().data() );
    if (list->isEmpty() || list->first().isEmpty()) {
        logError("LDAP userdata not complete! DisplayName- attribute '%1' is missing!",
                 config->get("auth_backend_ldap", "attribute_displayname") );
        return false;
    }
    QString displayName = list->first();
    logDebug("LDAP userdata: displayName is '%1'", displayName);

    // Mail- Adresse vom LDAP auslesen:
    //list = userLdapAttributes->getValues( config->get("auth_backend_ldap", "attribute_mail").toUtf8().data() );
    //if (list->isEmpty() || list->first().isEmpty()) {
    //    logError("LDAP userdata not complete! Mail address- attribute '%1' is missing!",
    //             config->get("auth_backend_ldap", "attribute_mail"));
    //    return false;
    //}
    //QString mail = list->first();
     QString mail = "no@mail.at";
     logDebug("LDAP userdata: mail is '%1'", mail);

    /**
      * Nun haben wir alle nötigen LDAP Daten....
      * Jetzt können wir schauen, ob der Benutzer- Account in unserer Datenbank existiert...
      */

    logNotice("Check, if ldapId '%1' already exists in the database...", ldapId);
    if (!dbUser::checkExistsByLdapId(ldapId)) {

        logNotice("ldapid '%1' does not exists in the intranet database, its a new user account...", ldapId);

        if (dbUser::checkExistsByUsername(username)) {
            logFatal("Could not create the user account. Username %1 already exists, but have an other ldapid! Is the username duplicate?",
                     username);
            return false;
        }

        // Wir legen einen Account an:
        dbUserModel userCreate;
        userCreate.setUsername(username);
        userCreate.setLdapId(ldapId);
        userCreate.setSurName(surName);
        userCreate.setGivenName(givenName);
        userCreate.setDisplayName(displayName);
        userCreate.setMail(mail);

        // Account erstellen:
        DatabaseQuery q = userCreate.create();
    }


    if (! _user->loadByLdapId(ldapId) ) {
        logFatal("Could not load userdata from database! ldapid '%1' not found...", ldapId);
        return false;
    }

    UtilUsers::reloadMemberships(_user->getColumn("id").toInt());

    logNotice("User login '%1' successful!", username);

    return true;
}

void TcpCommand_main_userLogin::postExec(const QVariant &returnStatus) {

    TcpCommandsHelper::userLoginResponse response;
    response.success = returnStatus.toBool();

    _streamWrite(response);
}
