#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETBACKUPSTATEOFDAY_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETBACKUPSTATEOFDAY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getBackupStateOfDay : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getBackupStateOfDay();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETBACKUPSTATEOFDAY_H


