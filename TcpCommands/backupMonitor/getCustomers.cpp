#include "TcpCommands/backupMonitor/getCustomers.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Utils/UtilBackupMonitor.h"

TcpCommand_backupMonitor_getCustomers::TcpCommand_backupMonitor_getCustomers() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_backupMonitor_getCustomers::exec() {


    _streamWrite(UtilBackupMonitor::getCustomers());

    return QVariant();
}

