#include "TcpCommands/backupMonitor/getCustomersFactory.h"

#include "TcpCommands/backupMonitor/getCustomers.h"

TcpCommandFactory_backupMonitor_getCustomers::TcpCommandFactory_backupMonitor_getCustomers()
    : TcpAbstractCommandFactory("backupMonitor", "getCustomers")
{

}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getCustomers::createCommand() {

    return new TcpCommand_backupMonitor_getCustomers;
}

