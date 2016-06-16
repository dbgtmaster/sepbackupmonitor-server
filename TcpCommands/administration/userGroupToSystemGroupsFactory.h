#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_userGroupToSystemGroups : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_userGroupToSystemGroups();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H

