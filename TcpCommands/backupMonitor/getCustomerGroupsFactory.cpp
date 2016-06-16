#include "TcpCommands/backupMonitor/getCustomerGroupsFactory.h"

#include "TcpCommands/backupMonitor/getCustomerGroups.h"

TcpCommandFactory_backupMonitor_getCustomerGroups::TcpCommandFactory_backupMonitor_getCustomerGroups()
    : TcpAbstractCommandFactory("backupMonitor", "getCustomerGroups")
{
    
}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getCustomerGroups::createCommand() {
    
    return new TcpCommand_backupMonitor_getCustomerGroups;
}


