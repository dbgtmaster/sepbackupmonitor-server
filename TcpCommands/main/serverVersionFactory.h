#ifndef TCPCOMMANDFACTORY_MAIN_SERVERVERSION_H
#define TCPCOMMANDFACTORY_MAIN_SERVERVERSION_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_main_serverVersion : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_main_serverVersion();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_MAIN_SERVERVERSION_H
