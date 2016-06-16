#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H


