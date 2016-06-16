#include "TcpCommands/backupMonitor/getJobFactory.h"

#include "TcpCommands/backupMonitor/getJob.h"

TcpCommandFactory_backupMonitor_getJob::TcpCommandFactory_backupMonitor_getJob()
    : TcpAbstractCommandFactory("backupMonitor", "getJob")
{

}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getJob::createCommand() {

    return new TcpCommand_backupMonitor_getJob;
}

