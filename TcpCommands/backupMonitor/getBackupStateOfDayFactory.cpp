#include "TcpCommands/backupMonitor/getBackupStateOfDayFactory.h"

#include "TcpCommands/backupMonitor/getBackupStateOfDay.h"

TcpCommandFactory_backupMonitor_getBackupStateOfDay::TcpCommandFactory_backupMonitor_getBackupStateOfDay()
    : TcpAbstractCommandFactory("backupMonitor", "getBackupStateOfDay")
{
    
}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getBackupStateOfDay::createCommand() {
    
    return new TcpCommand_backupMonitor_getBackupStateOfDay;
}


