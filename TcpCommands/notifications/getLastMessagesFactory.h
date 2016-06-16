#ifndef TCPCOMMANDFACTORY_NOTIFICATIONS_GETLASTMESSAGES_H
#define TCPCOMMANDFACTORY_NOTIFICATIONS_GETLASTMESSAGES_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_notifications_getLastMessages : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_notifications_getLastMessages();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_NOTIFICATIONS_GETLASTMESSAGES_H


