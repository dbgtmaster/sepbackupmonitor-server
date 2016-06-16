#ifndef TCPCOMMANDFACTORY_LOGGEDINUSER_HASTCPPERMISSION_H
#define TCPCOMMANDFACTORY_LOGGEDINUSER_HASTCPPERMISSION_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_loggedInUser_hasTcpPermission : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_loggedInUser_hasTcpPermission();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_LOGGEDINUSER_HASTCPPERMISSION_H


