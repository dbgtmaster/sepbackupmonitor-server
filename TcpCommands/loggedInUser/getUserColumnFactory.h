#ifndef TCPCOMMANDFACTORY_LOGGEDINUSER_GETUSERCOLUMN_H
#define TCPCOMMANDFACTORY_LOGGEDINUSER_GETUSERCOLUMN_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_loggedInUser_getUserColumn : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_loggedInUser_getUserColumn();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_LOGGEDINUSER_GETUSERCOLUMN_H
