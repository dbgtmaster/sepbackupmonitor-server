#include "TcpCommands/system/getAllSystemGroups.h"

#include <QList>
#include <QPair>

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

TcpCommand_system_getAllSystemGroups::TcpCommand_system_getAllSystemGroups() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_system_getAllSystemGroups::exec() {

    // Soll auch die Beschreibung der Systemgruppe zum Client übertragen werden?
    bool sendDescription;
    _streamRead(sendDescription);

    DatabaseQuery q = _db->query("SELECT id, name "+ ((sendDescription) ? QString(",description") : ("") )
                                 +" FROM system_groups");
    while(q.next()) {
        _streamWrite( quint32( q.row("id").toInt() ) );
        _streamWrite( q.row("name").toString() );

        if (sendDescription)
            _streamWrite( q.row("description").toString() );
    }

    return QVariant();
}

