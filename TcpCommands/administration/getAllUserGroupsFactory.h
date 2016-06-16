#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_GETALLUSERGROUPS_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_GETALLUSERGROUPS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_getAllUserGroups : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_getAllUserGroups();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_GETALLUSERGROUPS_H

