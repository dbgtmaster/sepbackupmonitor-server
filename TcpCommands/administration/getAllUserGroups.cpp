#include "TcpCommands/administration/getAllUserGroups.h"

#include <QStringList>

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"

TcpCommand_administration_getAllUserGroups::TcpCommand_administration_getAllUserGroups() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_administration_getAllUserGroups::exec() {

    QStringList validColumns;
    validColumns << "id" << "name";

    // Welche Spalten sollen ausgelesen werden??
    QStringList getColumns;
    _streamRead(getColumns);

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
    DatabaseQuery q = _db->query("SELECT " + getColumns.join(",") + " FROM user_groups");
    while (q.next()) {

        _streamWrite(q.getAllColumns());
    }

    return QVariant();
}

