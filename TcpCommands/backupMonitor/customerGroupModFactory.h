#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_CUSTOMERGROUPMOD_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_CUSTOMERGROUPMOD_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_customerGroupMod : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_customerGroupMod();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_CUSTOMERGROUPMOD_H


