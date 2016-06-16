#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPDELETE_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPDELETE_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_userGroupDelete : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_userGroupDelete();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPDELETE_H

