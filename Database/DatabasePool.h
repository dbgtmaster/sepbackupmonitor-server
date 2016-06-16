/**
  * Stellt einen Pool bereit, um persistente Datenbankverbindungen zu verwalten.
  */

#ifndef DATABASEPOOL_H
#define DATABASEPOOL_H

#include "Database/Database.h"

#include <QSharedPointer>
#include <QWeakPointer>
#include <QList>
#include <QDebug>

#include <QMutex>

typedef QSharedPointer<Database> DB;

class DatabasePool
{
    friend void databasePoolDestroyFreedConnection(Database*);

private:

    static int _minConnections;        // Wie viele Verbindungen müssen initialisiert sein?
    static int _maxConnections;        // Wie viele Verbindungen soll der Pool maximal bereit stellen

    static int _countConnections;      // Anzahl vorhandene DB Verbindugnen;

    // Verbindungsdaten zur Datenbank (um, wenn nötig, weitere Verbindungen herstellen zu können...
    static QString _databaseHostname;
    static QString _databaseUsername;
    static QString _databasePassword;
    static QString _databaseName;

    // Mutex zum bekommen der Datenbankverbindung:
    static QMutex _getDatabaseConnectionMutex;

    // Mit mit allen Verfügbaren Verbindungen, welche noch nicht freigegeben wurden:
    static QList< Database* > _pool;

    // Eine Liste, welchem Thread welche Datenbankverbindung freigegeben wurde..
    // QT::HANDLE: Die ID des Threads
    struct FreedConnectionInformations {
        FreedConnectionInformations() : db(0), count(0) { }
        FreedConnectionInformations(Database *_db, quint32 _count) : db(_db), count(_count) { }
        Database *db;       // Die Verbindung zur Datenbank
        quint32 count;      // Zähler, wie oft diese Verbindung via "get()" freigegeben wurde.
    };
    static QMap<Qt::HANDLE, FreedConnectionInformations> _freedDatabaseConnections;

    // Erstellt eine weitere Verbindung im Pool:
    static void _createPoolConnection();

    // Gibt die nächste DB Verbindung zurück:
    static DB _getNextConnection();

    // Gibt zu einem bestimmten Thread die Datenbankverbindung zurück
    static DB _getThreadConnection();

    DatabasePool();
public:

    // Initialisiert den Pool
    // int min:     Anzahl der Mindest- Verbindungen
    // int max:     Anzahl der maximalen Verbindungen:
    static void init(int min, int max, const QString &hostname, const QString &username, const QString &password, const QString &database);

    // Räumt alle Verbindungen auf...
    static void clear();

    // Gibt eine freie Verbindung zurück...
    static DB get();

    inline static int getMinConnections();             // Wie viele DB Verbindugen mindestens erstellt werden.
    inline static int getMaxConnections();             // Wie viele DB Verbindugen maximal erstellt werden.
    inline static int getFreeConnections();          // Wie viele Verbindungen im Pool momentan Verf�bar sind.
    inline static int getTotalManagedConnections();    // Wie viele Verbindungen erstellt wurden.
};

int DatabasePool::getMinConnections() {
    _getDatabaseConnectionMutex.lock();
    int r = _minConnections;
    _getDatabaseConnectionMutex.unlock();
    return r;
}

int DatabasePool::getMaxConnections() {
    _getDatabaseConnectionMutex.lock();
    int r = _maxConnections;
    _getDatabaseConnectionMutex.unlock();
    return r;
}


int DatabasePool::getFreeConnections() {
    _getDatabaseConnectionMutex.lock();
    int r = _pool.count();
    _getDatabaseConnectionMutex.unlock();
    return r;
}

int DatabasePool::getTotalManagedConnections() {
    _getDatabaseConnectionMutex.lock();
    int r = _pool.count() + _freedDatabaseConnections.count();
    _getDatabaseConnectionMutex.unlock();
    return r;
}

#endif // DATABASEPOOL_H
