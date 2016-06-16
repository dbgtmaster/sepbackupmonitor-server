#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETLOG_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETLOG_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getLog : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getLog();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETLOG_H

