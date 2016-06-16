#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERGROUPS_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERGROUPS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getCustomerGroups : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getCustomerGroups();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERGROUPS_H


