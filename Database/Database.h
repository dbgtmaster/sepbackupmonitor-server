#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>

#include "Database/DatabaseTypeDefs.h"

#include <QHash>
#include <QMap>

#include "Database/DatabaseQuery.h"

class Database
{

private:
    QSqlDatabase _db;

    // Da jede neue Datenbankverbindung einen eindeutigen Namen brauchen,
    // zählen wir mit einem Zähler mit, um doppelte Datenbanknamen zu vermeiden.
    static int _dbConnId;

    // TRUE, wenn eine Transaktion gestartet wurde, ansonsten FALSE
    bool _transactionStarted;
public:
    Database();
    ~Database();

    void connect(const QString &hostname, const QString &username, const QString &password,
                 const QString &database);


    // Gibt ein leeres DatabaseQuery Objekt zurück...
    DatabaseQuery getQuery();
    // ACHTUNG: query() führt die SQL direkt aus und gibt das DatabaseQuery Objekt zurück.
    DatabaseQuery query(const QString& sql);

    QString escape(const QVariant &string);

    // Umgang mit Transaktionen...
    bool transaction();
    bool commit();
    bool rollback();
    bool transactionStarted();      // true, wenn eine Transaktion gestartet wurde, ansonsten false.

    /**
      * Einige Hilfsmethoden zum ausführen von Queries:
      */
    // Select Methoden
    // Diese Methoden lesen maximal einen Datensatz aus!!!
    mapColumns select(const QString &columns,
                         hashColumns &conditions, const QString &tableName );
    mapColumns select(const QString &columns,
                         const QString &conditionColumn, const QVariant &conditionValue,
                         const QString &tableName);

    // Überprüft, ob ein bestimmter Eintrag existert...
    // Sollte in der abgeleiteten Klasse eine Methode wie model->checkById()
    // existieren, so Methode erstellen und Anfrage dieser Methode weitergeben...
    // Wird kein QString tableName angegeben, so wird die in der Standard- Tabellenname _tableName verwendet.
    bool checkExists(const QVariant &id, const QString &column,
                     const QString &tableName, bool forUpdate = false);

    // Fügt einen neuen Datensatz ein.
    // Daten müssen im folgenden Format vorliegen:
    // QHash<QString, QVariant> bzw. QMap<QString, QVariant>
    // template "I" erwartet den Const- Iterator der Contrainer- Klasse
    // Parameter begin & end erwarten einen const_iterator der Klasse QHash oder QMap
    // Wird kein QString tableName angegeben, so wird die der Standard- Tabellenname _tableName verwendet.
    // returnColumn wird benutzt, um die ID des eingefügten Datensatzes zu ermitteln, Als Parameter wird
    // die Spalte erwartet (zB. id). Im DatabaseQuery kann dann über die Spalte "id" der Wert ausgelesen werden.
    // Wenn bestimmte werte nicht escaped werden sollen, so wird anstatt eines "QStrings" ein "QStringList() << query" übergeben. QStringList darf nur einen Wert haben!
    template <class I>
    DatabaseQuery insert(I begin, I end, const QString &tableName, const QString &returnColumn);
    DatabaseQuery insert(const hashColumns &cols, const QString &tableName);
    DatabaseQuery insert(const mapColumns &cols, const QString &tableName);
    DatabaseQuery insert(const hashColumns &cols, const QString &tableName, const QString &returnColumn);
    DatabaseQuery insert(const mapColumns &cols, const QString &tableName, const QString &returnColumn);

    // Ändert einen Datensatz:
    // Daten müssen im folgenden Format vorliegen:
    // QHash<QString, QVariant> bzw. QMap<QString, QVariant>
    // template "I" erwartet den Const- Iterator der Contrainer- Klasse
    // Parameter begin & end erwarten einen const_iterator der Klasse QHash oder QMap
    // Wird kein QString tableName angegeben, so wird die in der Standard- Tabellenname _tableName verwendet.
    // Wenn bestimmte werte nicht escaped werden sollen, so wird anstatt eines "QStrings" ein "QStringList() << query" übergeben. QStringList darf nur einen Wert haben!
    // Die conditions (Bedindungen) werden mit AND verknüpft.
    template <class I>
    DatabaseQuery update(I begin, I end, hashColumns &conditions, const QString &tableName );
    template <class I>
    DatabaseQuery update(I begin, I end,
                         const QString &tableName,
                         const QString &conditionColumn, const QVariant &conditionValue );
    DatabaseQuery update(const mapColumns &cols, const QString &tablename,
                         QString conditionColumn, QVariant conditionValue);

    DatabaseQuery remove(const QString &tableName,
                         const QString &conditionColumn, const QVariant &conditionValue);
};


// Exception, wenn ein SQL Fehler aufgetreten ist...
class SqlException {
private:
    QString _sql;
    QSqlError _error;

public:
    SqlException(const QString &sql, const QSqlError &error);

    inline const QString& sql() const {
        return _sql;
    }
    inline const QSqlError &error() const {
        return _error;
    }
};

/**
  * Template Implementierungen
  */

// Fügt einen neuen Datensatz ein:
template <class I>
DatabaseQuery Database::insert(I begin, I end, const QString &tableName, const QString &returnColumn) {

    QString sql;
    sql = "INSERT INTO " + tableName + " (";

    // Alle Spaltennamen einfügen:
    for (I i = begin; i != end; i++) {

        if (i != begin) sql += ",";
        sql += i.key();
    }

    sql += ") VALUES (";

    // Die Werte der Spalten einfügen:
    for (I i = begin; i != end; i++) {

        if (i != begin) sql += ",";

        sql += escape( i.value() );
    }

    sql += ")";

    if (returnColumn.count() > 0) {
        sql += " returning " + returnColumn;
    }

    return query(sql);
}

// UPDATE mit mehreren Bedingungen
template <class I>
DatabaseQuery Database::update(I begin, I end, hashColumns &conditions, const QString &tableName ) {

    QString sql;
    sql = "UPDATE " + tableName + " SET ";

    // Alle Spaltennamen + deren Werte einfügen:
    for (I i = begin; i != end; i++) {

        if (i != begin) sql += ",";
        sql += i.key() + "=" + escape(i.value());
    }

    // Condition:
    sql += " WHERE ";
    for (hashColumns::const_iterator i = conditions.begin(); i != conditions.end(); i++) {

        if (i != conditions.begin()) sql += " AND ";
        sql += i.key() + "=" + escape( i.value() );
    }

    return query(sql);
}

// Überladene Version von _update()
template <class I>
inline DatabaseQuery Database::update(I begin, I end,
                                     const QString &tableName,
                                     const QString &conditionColumn, const QVariant &conditionValue ) {

    hashColumns hash;
    hash.insert(conditionColumn, conditionValue);
    return update<I>(begin, end, hash, tableName);
}

#endif // DATABASE_H
