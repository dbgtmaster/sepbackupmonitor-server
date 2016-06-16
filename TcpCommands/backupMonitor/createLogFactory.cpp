#include "TcpCommands/backupMonitor/createLogFactory.h"

#include "TcpCommands/backupMonitor/createLog.h"

TcpCommandFactory_backupMonitor_createLog::TcpCommandFactory_backupMonitor_createLog()
    : TcpAbstractCommandFactory("backupMonitor", "createLog")
{

}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_createLog::createCommand() {

    return new TcpCommand_backupMonitor_createLog;
}

