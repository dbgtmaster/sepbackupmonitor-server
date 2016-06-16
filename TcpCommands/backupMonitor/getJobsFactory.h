#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOBS_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOBS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getJobs : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getJobs();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOBS_H

