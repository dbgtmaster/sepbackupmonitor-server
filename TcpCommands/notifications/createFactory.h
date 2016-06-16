#ifndef TCPCOMMANDFACTORY_NOTIFICATIONS_CREATE_H
#define TCPCOMMANDFACTORY_NOTIFICATIONS_CREATE_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_notifications_create : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_notifications_create();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_NOTIFICATIONS_CREATE_H


