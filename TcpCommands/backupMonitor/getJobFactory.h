#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOB_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOB_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getJob : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getJob();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOB_H

