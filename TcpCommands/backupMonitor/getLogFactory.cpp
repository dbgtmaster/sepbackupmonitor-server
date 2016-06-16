#include "TcpCommands/backupMonitor/getLogFactory.h"

#include "TcpCommands/backupMonitor/getLog.h"

TcpCommandFactory_backupMonitor_getLog::TcpCommandFactory_backupMonitor_getLog()
    : TcpAbstractCommandFactory("backupMonitor", "getLog")
{

}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getLog::createCommand() {

    return new TcpCommand_backupMonitor_getLog;
}

