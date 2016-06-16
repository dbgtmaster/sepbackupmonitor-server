#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_CUSTOMERGROUPDELETE_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_CUSTOMERGROUPDELETE_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_customerGroupDelete : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_customerGroupDelete();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_CUSTOMERGROUPDELETE_H


