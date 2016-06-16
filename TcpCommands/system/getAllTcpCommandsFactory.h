#ifndef TCPCOMMANDFACTORY_SYSTEM_GETALLTCPCOMMANDS_H
#define TCPCOMMANDFACTORY_SYSTEM_GETALLTCPCOMMANDS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_system_getAllTcpCommands : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_system_getAllTcpCommands();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_SYSTEM_GETALLTCPCOMMANDS_H

