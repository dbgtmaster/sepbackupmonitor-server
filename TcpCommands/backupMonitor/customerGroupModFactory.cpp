#include "TcpCommands/backupMonitor/customerGroupModFactory.h"

#include "TcpCommands/backupMonitor/customerGroupMod.h"

TcpCommandFactory_backupMonitor_customerGroupMod::TcpCommandFactory_backupMonitor_customerGroupMod()
    : TcpAbstractCommandFactory("backupMonitor", "customerGroupMod")
{
    
}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_customerGroupMod::createCommand() {
    
    return new TcpCommand_backupMonitor_customerGroupMod;
}


