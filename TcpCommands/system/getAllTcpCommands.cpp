#include "TcpCommands/system/getAllTcpCommands.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"

TcpCommand_system_getAllTcpCommands::TcpCommand_system_getAllTcpCommands() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_system_getAllTcpCommands::exec() {

    DatabaseQuery q = _db->query("SELECT id, group_name, action_name, system_group_id FROM tcp_actions");
    while(q.next()) {
        _streamWrite( quint32( q.row("id").toInt() ) );                 // Id des Kommandos
        _streamWrite( q.row("group_name").toByteArray() );              // Gruppenzuordnung des Kommandos
        _streamWrite( q.row("action_name").toByteArray() );             // Kommandoname
        _streamWrite( quint32( q.row("system_group_id").toInt() ) );    // Fehler Systemgruppe das Kommando zugeordnet ist
    }

    return QVariant();
}


