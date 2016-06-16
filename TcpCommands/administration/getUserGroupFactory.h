#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_GETUSERGROUP_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_GETUSERGROUP_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_getUserGroup : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_getUserGroup();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_GETUSERGROUP_H

