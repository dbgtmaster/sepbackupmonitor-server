#include "TcpCommands/backupMonitor/getJobsFactory.h"

#include "TcpCommands/backupMonitor/getJobs.h"

TcpCommandFactory_backupMonitor_getJobs::TcpCommandFactory_backupMonitor_getJobs()
    : TcpAbstractCommandFactory("backupMonitor", "getJobs")
{

}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getJobs::createCommand() {

    return new TcpCommand_backupMonitor_getJobs;
}

