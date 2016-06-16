#ifndef DBABSTRACTMODEL_H
#define DBABSTRACTMODEL_H

#include <Logger/Logger.h>

#include <Database/DatabasePool.h>

#include <QString>
#include <QVariant>
#include <QList>
#include <QPair>

#include <QMap>
#include <QHash>

class QVariant;

class dbAbstractModel
{
protected:

    // In diesem Map werden die Spalten + Werte aus der Datenbank gespeichert, welche das abgeleitete
    // Model verwaltet.
    mapColumns _dbData;

    // Verbindung zur Datenbank
    DB _db;

    bool _isValid;

public:
    dbAbstractModel();

    QVariant getColumn(const QString &name);

    // Ob dieses Objekt ein gültiges Objekt ist???
    // Wenn zB. beim laden eines bestimmtes Accounts der Account nicht in der Datenbank exisitert...
    // oder wenn noch gar kein Account geladen wurde..
    bool isValid();

    // Setzt die Gültigkeit dieses Objektes und gibt den übergebenen bool wieder zurück...
    bool setValid(bool t);
};

/**
  * Exception, wenn die eingegebenen Daten nicht gültig sind...
  * Diese Exception kann von jedem Model geschmissen werden, daher
  * die Models immer in einem Try- Block fassen!
  */
class dbModelException {
private:
    QString _message;
public:
    dbModelException(const QString &message);

    QString& getMessage();
};

#endif // DBABSTRACTMODEL_H
