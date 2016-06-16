#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_CREATELOG_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_CREATELOG_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_createLog : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_createLog();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_CREATELOG_H

