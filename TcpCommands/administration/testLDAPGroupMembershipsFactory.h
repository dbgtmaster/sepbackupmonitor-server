#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_testLDAPGroupMemberships : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_testLDAPGroupMemberships();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H

