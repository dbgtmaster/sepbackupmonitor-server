#include "DatabaseQuery.h"

#include <QVariant>
#include <QSqlRecord>

#include "Logger/Logger.h"

#include "Database/Database.h"

DatabaseQuery::DatabaseQuery(const QSqlDatabase &db) :
    _query(db)
{
}

DatabaseQuery DatabaseQuery::exec(const QString &sql) {

    logDebug("SQL: " + sql);

    if (! _query.exec(sql) ) {
        throw SqlException(_query.lastQuery(), _query.lastError());
    }

    return *this;
}

void DatabaseQuery::exec() {

    logDebug("SQL: " + _query.lastQuery());

    if (! _query.exec() ) {
        throw SqlException(_query.lastQuery(), _query.lastError());
    }
}

QVariant DatabaseQuery::boundValue(const QString &placeHolder) const {
    return _query.boundValue(placeHolder);
}

void DatabaseQuery::bindValue(const QString &placeholder, const QVariant &val) {
    _query.bindValue(placeholder, val);
}

bool DatabaseQuery::prepare(const QString &query) {
    return _query.prepare(query);
}

bool DatabaseQuery::next(){
    return _query.next();
}

QVariant DatabaseQuery::row(const QString &row) {
    return _query.value( _query.record().indexOf(row) );
}

QVariant DatabaseQuery::row(int i) {
    return _query.value( i );
}

QString DatabaseQuery::rowName(int i) {
    return _query.record().fieldName(i);
}

int DatabaseQuery::columnsCount() {
    return _query.record().count();
}

int DatabaseQuery::count() {
    return _query.size();
}

QVariant DatabaseQuery::fetchCol(const QString &name, bool *ok) {

    if (ok != 0) {
        *ok = next();
    }
    else {
        next();
    }
    return row(name);
}

int DatabaseQuery::numAffectedRows() {
    return _query.numRowsAffected();
}

hashColumns DatabaseQuery::getAllColumns() {

    hashColumns c;

    int count = columnsCount();
    for (int i = 0; i < count; i++) {

        c[rowName(i)] = row(i);
    }

    return c;
}

QList<hashColumns> DatabaseQuery::fetchAll() {

    QList<hashColumns> list;

    while (next()) {
        list.append( getAllColumns() );
    }

    return list;
}
