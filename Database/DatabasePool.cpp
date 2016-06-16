#include "DatabasePool.h"

#include "Logger/Logger.h"

#include <QMutexLocker>
#include <QThread>
#include <QDebug>


#include <unistd.h>

QList< Database* > DatabasePool::_pool;
QMap<Qt::HANDLE, DatabasePool::FreedConnectionInformations> DatabasePool::_freedDatabaseConnections;
QMutex DatabasePool::_getDatabaseConnectionMutex;

int DatabasePool::_countConnections = 0;
int DatabasePool::_minConnections = 0;
int DatabasePool::_maxConnections = 0;

QString DatabasePool::_databaseHostname;
QString DatabasePool::_databaseUsername;
QString DatabasePool::_databasePassword;
QString DatabasePool::_databaseName;

DatabasePool::DatabasePool()
{
}

// Funktion wird aufgerufen, wenn die Datenbankverbindung wieder freigegeben wird...
// @see QSharedPointer<Database>.clear();
void databasePoolDestroyFreedConnection(Database *db) {

    logDebug("databasePoolDestroyFreedConnection(): lock mutex..");
    QMutexLocker mutexLocker(&DatabasePool::_getDatabaseConnectionMutex);

    DatabasePool::FreedConnectionInformations &databaseData = DatabasePool::_freedDatabaseConnections[QThread::currentThreadId()];
    databaseData.count--;

    logDebug("databasePoolClearSharedPointer(): freed thread database connection counter: %1", QString::number(databaseData.count));

    if (databaseData.count == 0) {

        logDebug("databasePoolClearSharedPointer(): database connection not longer used... Re- insert connection to pool.");

        // Es ist für diesen Thread keine Datenbankverbindung mehr im Umlauf, daher können wir in den Pool hinzufügen...
        if (databaseData.db->transactionStarted()) databaseData.db->rollback();
        DatabasePool::_pool.append( databaseData.db );
        DatabasePool::_freedDatabaseConnections.remove(QThread::currentThreadId());
    }
}

void DatabasePool::_createPoolConnection() {

    Database *db = new Database;
    db->connect(_databaseHostname, _databaseUsername, _databasePassword, _databaseName);

    _countConnections++;

    // Verbindung zum Pool hinzufügen:
    _pool.append(db);
}

void DatabasePool::init(int min, int max, const QString &hostname, const QString &username, const QString &password, const QString &database) {

    logNotice("DatabasePool::init(): init database pool with %1 connections", QString::number(min) );

    _minConnections = min;
    _maxConnections = max;

    _databaseHostname = hostname;
    _databaseUsername = username;
    _databasePassword = password;
    _databaseName = database;

    for (int i = 1; i <= min; i++) {

        logDebug("Create pool connection %1/%2", QString::number(i), QString::number(min));
        _createPoolConnection();
    }
}

void DatabasePool::clear() {

    // Zu allen bereits verbundenen Datenbanken die Verbindung trennen:
    QList< Database* >::Iterator it;
    for (it = _pool.begin(); it != _pool.end(); it++) {

        // Destruktor trennt Verbindung zur Datenbank:
        delete (*it);
    }

    _countConnections = 0;
    _minConnections = 0;
    _maxConnections = 0;
}

DB DatabasePool::_getNextConnection() {

    logDebug("DatabasePool::_getNextConnection(): Return next free connection.");

    DB db = DB( _pool.takeLast(), databasePoolDestroyFreedConnection);

    // Speichern, dass diesem Thread diese Verbindung zugeordnet wurde:
    _freedDatabaseConnections.insert(QThread::currentThreadId(), FreedConnectionInformations(db.data(), 1));

    return db;
}

DB DatabasePool::_getThreadConnection() {

    logDebug("DatabasePool::_getThreadConnection(): Return the same freed database connection for the thread.");

    FreedConnectionInformations &freedData = _freedDatabaseConnections[QThread::currentThreadId()];
    freedData.count++;

    return DB(freedData.db, databasePoolDestroyFreedConnection);
}

DB DatabasePool::get() {


    logDebug("DatabasePool::getConnection(): Get database connection..");

    while (true) {

        logDebug("DatabasePool::getConnection(): lock mutex..");
        QMutexLocker mutexLocker(&_getDatabaseConnectionMutex);
        logDebug("DatabasePool::getConnection(): mutex locked successful..");

        // Überprüfen, ob diesem Thread bereits eine Datenbankverbindung hat, wenn ja, dann die selbe wieder zurückgeben:
        if (_freedDatabaseConnections.contains(QThread::currentThreadId())) {
            logDebug("DatabasePool::getConnection(): Thread already have a database connection, returns the same...");
            return _getThreadConnection();
        }

        try {

            logDebug("DatabasePool::getConnection(): Free connections in pool: %1", QString::number(_pool.size()));
            if (_pool.size() > 0) {

                // Verfügbare Verbindung aus dem Pool holen:
                logDebug("DatabasePool::getConnection(): Return the next free connection from the pool.");
                return _getNextConnection();
            }

            // Da keine Verbindung verfügbar, überprüfen, ob das maximale Limit an Verbindungen erreicht wurde?
            if (_maxConnections > _countConnections) {

                logNotice("DatabasePool::getConnection(): No db connection available, max %1, created %2, create a new...", QString::number(_maxConnections), QString::number(_countConnections));
                _createPoolConnection();

                return _getNextConnection();
            }

            logError("DatabasePool::getConnection(): No connection available (max %1, created %2), sleep 50ms..", QString::number(_maxConnections), QString::number(_countConnections));
        }
        catch (SqlException *e) {

            /**
              * @TODO: Defekte Datenbankverbindungen filtern???
              */

            throw e;
        }

        // Wenn keine Datenbankverbindung zurückgegeben werden konnte, so Mutex entsperren und 50ms schlafen...
        mutexLocker.unlock();
        usleep(50);
    }
}
