#include "Database.h"

#include <stdlib.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlDriver>

#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"
#include <QDebug>

// Da jede neue Datenbankverbindung einen eindeutigen Namen brauchen,
// zählen wir mit einem Zähler mit, um doppelte Datenbanknamen zu vermeiden.
int Database::_dbConnId = 0;

Database::Database() : _transactionStarted(false)
{
}

Database::~Database() {

    _db.close();
    logNotice("Connection to database closed.");
}

void Database::connect(const QString &hostname, const QString &username,
                       const QString &password, const QString &database) {

    logNotice("Connect to database: hostname '%1', username '%2', database '%3'", hostname, username, database);

    Q_ASSERT_X(!_db.isValid(), "Database::connect()", "Database already connected!");

    _db = QSqlDatabase::addDatabase( "QPSQL", QString::number(_dbConnId++) + QString::number(rand()) );

    _db.setHostName(hostname);
    _db.setUserName(username);
    _db.setPassword(password);
    _db.setDatabaseName(database);

    if (!_db.open()) {
        logFatal("Could not connect to psql database server :(!");
        throw SqlException("Connecting to database", _db.lastError());
    }

    logNotice("Connection to database successful!");
}

mapColumns Database::select(const QString &columns, hashColumns &conditions, const QString &tableName) {

    QString sql;
    sql = "SELECT "+ columns +" FROM "+ tableName + " WHERE ";

    for (hashColumns::const_iterator i = conditions.begin(); i != conditions.end(); i++) {

        if (i != conditions.begin()) sql += " AND ";
        sql += i.key() + "=" + escape( i.value() );
    }

    DatabaseQuery q = query(sql);

    // Und nun alle Spalten ermitteln:
    if ( ! q.next() ) return mapColumns();

    mapColumns map;
    for (int i = 0; i < q.columnsCount(); i++) {
        map[ q.rowName(i) ] = q.row(i);
    }

    return map;
}

// Überladene select() Version...
mapColumns Database::select(const QString &columns,
                                      const QString &conditionColumn, const QVariant &conditionValue,
                                      const QString &tableName) {
    hashColumns hash;
    hash.insert(conditionColumn, conditionValue);
    return select(columns, hash, tableName);
}

DatabaseQuery Database::getQuery() {
    return DatabaseQuery(_db);
}

DatabaseQuery Database::query(const QString &sql) {

    return getQuery().exec(sql);
}

bool Database::transaction() {

    if (!_db.transaction()) {
        throw SqlException("Database::transaction", _db.lastError());
    }
    _transactionStarted = true;
    return true;
}

bool Database::commit() {

    if (!_db.commit()) {
        throw SqlException("Database::commit()", _db.lastError());
    }
    _transactionStarted = false;
    return true;
}

bool Database::rollback() {

    if (!_db.rollback()) {
        throw SqlException("Database::rollback()", _db.lastError());
    }
    _transactionStarted = false;
    return true;
}

bool Database::transactionStarted() {
    return _transactionStarted;
}

QString Database::escape(const QVariant &string) {

    if (string.type() == QVariant::Bool) {
        logDebug((string.toBool()) ? "true" : "false");
        return (string.toBool()) ? "true" : "false";
    }

    if (string.type() == QVariant::StringList) {
        return string.toStringList().takeFirst();
    }

    QSqlField field(QString(), string.type());
    field.setValue(string);
    return QSqlDatabase::database().driver()->formatValue( field );
}

bool Database::checkExists(const QVariant &id, const QString &column,
                                   const QString& tableName, bool forUpdate) {

    QString sql = "SELECT COUNT(*) AS count FROM "+ tableName +" WHERE ";

    if (id.type() == QVariant::String) {
        sql += "LOWER("+ column +") = LOWER("+ escape(id) +")";
    }
    else {
        sql += column +" = "+ escape(id);
    }

    if (forUpdate) {
        sql += " FOR UPDATE";
    }

    DatabaseQuery q = query(sql);
    q.next();
    return (q.row("count").toInt() != 0);
}

DatabaseQuery Database::insert(const hashColumns &cols, const QString &tableName) {
    return insert< hashColumns::ConstIterator >(cols.begin(), cols.end(), tableName, QString());
}

DatabaseQuery Database::insert(const mapColumns &cols, const QString &tableName) {
    return insert< mapColumns::ConstIterator >(cols.begin(), cols.end(), tableName, QString());
}

DatabaseQuery Database::insert(const hashColumns &cols, const QString &tableName, const QString &returnColumn) {
    return insert< hashColumns::ConstIterator >(cols.begin(), cols.end(), tableName, returnColumn);
}

DatabaseQuery Database::insert(const mapColumns &cols, const QString &tableName, const QString &returnColumn) {
    return insert< mapColumns::ConstIterator >(cols.begin(), cols.end(), tableName, returnColumn);
}

DatabaseQuery Database::update(const mapColumns &cols, const QString &tablename,
                               QString conditionColumn, QVariant conditionValue) {

    return update< mapColumns::ConstIterator >(cols.begin(), cols.end(), tablename, conditionColumn, conditionValue);
}

DatabaseQuery Database::remove(const QString &tableName,
                               const QString &conditionColumn, const QVariant &conditionValue) {

    return query("DELETE FROM " + tableName + " WHERE " + conditionColumn + " = " + escape(conditionValue));
}

SqlException::SqlException(const QString &sql, const QSqlError &error) : _sql(sql), _error(error) {

}
