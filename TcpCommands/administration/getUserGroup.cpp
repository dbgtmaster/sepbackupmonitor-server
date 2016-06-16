#include "TcpCommands/administration/getUserGroup.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"

TcpCommand_administration_getUserGroup::TcpCommand_administration_getUserGroup() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_administration_getUserGroup::exec() {

    QStringList validColumns;
    validColumns << "id" << "name";

    quint32 id;
    QStringList getColumns;         // Zu lesende Spalten
    bool getSystemGroups;           // Zuordnung zu den Systemgruppen auslesen?

    _streamRead(id);
    _streamRead(getColumns);
    _streamRead(getSystemGroups);

    logDebug("groupid is '%1'", QString::number(id) );

    if (getColumns.count() == 0) validColumns << "id" << "name";

    // Schauen, ob nur gültige Columns angegeben wurden...
    foreach(QString column, validColumns) {

        if (validColumns.indexOf(column) == -1) {
            logError("To get column '%1'' is forbitten, because i cannot found it in validColumns list!", column);
            closeSocket();
            return QVariant();
        }
    }

    // Spalten auslesen:
    DatabaseQuery q = _db->query("SELECT " + getColumns.join(",") + " FROM user_groups WHERE id = " + QString::number(id) );
    if (!q.next()) {
        logWarning("UserGroup with id '%1' not found. I return an empty column list...");
        _streamWrite(mapColumns());
        _streamWrite(QList<int>());
        return QVariant();
    }

    _streamWrite(q.getAllColumns());


    /**
      * Zuordnung zu den Systemgruppen auch auslesen?
      */
    if (!getSystemGroups) {
        logDebug("memberships to systemgroups not requestet by the client.");
        _streamWrite(QList<int>());
    }
    else {
        QList<quint32> list;

        logDebug("Client requestet the memberships to systemgroups, systemgroups ids are:");

        DatabaseQuery q = _db->query("SELECT system_group FROM groups_memberships WHERE user_group = " + QString::number(id) );
        while (q.next()) {

            logDebug("groupid '%1' is membership auf systemgroup '%2'",
                     QString::number(id), QString::number(q.row("system_group").toInt()));

            list.append(q.row("system_group").toInt());
        }

        _streamWrite(list);
    }

    return QVariant();
}

