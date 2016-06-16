#include "TcpCommands/administration/testLDAPGroupMembershipsFactory.h"

#include "TcpCommands/administration/testLDAPGroupMemberships.h"

TcpCommandFactory_administration_testLDAPGroupMemberships::TcpCommandFactory_administration_testLDAPGroupMemberships()
    : TcpAbstractCommandFactory("administration", "testLDAPGroupMemberships")
{

}

TcpAbstractCommand* TcpCommandFactory_administration_testLDAPGroupMemberships::createCommand() {

    return new TcpCommand_administration_testLDAPGroupMemberships;
}

