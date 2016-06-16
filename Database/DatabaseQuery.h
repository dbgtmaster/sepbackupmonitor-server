/**
  * Ein Objekt kann & darf nur über Database::getQuery() erzeugt werden !
  */

#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

#include <QSqlQuery>

#include "Database/DatabaseTypeDefs.h"

class QSqlDatabase;
class Database;

class DatabaseQuery
{

    // Um Objekte über die Database Klasse erzeugen zu können, da der Konstruktor der Klasse
    // DatabaseQuery private ist.
    friend class Database;

private:
    QSqlQuery _query;

    DatabaseQuery(const QSqlDatabase &db);

public:
    DatabaseQuery exec(const QString &sql);
    void exec();

    bool next();
    QVariant row(const QString &row);       // Wert einer Spalte erhalten
    QVariant row(int i);       // Wert einer Spalte erhalten
    int columnsCount();        // Anzahl der Spalten
    int count();               // Anzahl der Zeilen
    QString rowName(int i);

    // F�hrt ein next(), sowie return row(name) aus.
    // Dient als bessere Lesbarkeit, wenn zB. von einer SQL nur eine Spalte ben�tigt wird...
    // QString var = db->query("SELECT col FROM table WHERE id = x")->fetchCol("col").toString();
    // book *ok => speicher in die ok true, wenn ein Datensatz geliefert wurde, ansonsten false.
    QVariant fetchCol(const QString &name, bool *ok = 0);

    int numAffectedRows();

    bool prepare(const QString &query);
    void bindValue(const QString & placeholder, const QVariant & val);
    QVariant boundValue(const QString &placeHolder) const;

    // Gibt alle Spalten des aktuellen Datensatzes zurück...
    hashColumns getAllColumns();

    // Gibt die Spalten aller Ergebnisse in einer Liste zurück...
    QList< hashColumns > fetchAll();
};

#endif // DATABASEQUERY_H
