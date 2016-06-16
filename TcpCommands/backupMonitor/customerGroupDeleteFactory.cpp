#include "TcpCommands/backupMonitor/customerGroupDeleteFactory.h"

#include "TcpCommands/backupMonitor/customerGroupDelete.h"

TcpCommandFactory_backupMonitor_customerGroupDelete::TcpCommandFactory_backupMonitor_customerGroupDelete()
    : TcpAbstractCommandFactory("backupMonitor", "customerGroupDelete")
{
    
}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_customerGroupDelete::createCommand() {
    
    return new TcpCommand_backupMonitor_customerGroupDelete;
}


