#ifndef TCPCOMMANDFACTORY_SYSTEM_SERVERSTATISTICS_H
#define TCPCOMMANDFACTORY_SYSTEM_SERVERSTATISTICS_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_system_serverStatistics : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_system_serverStatistics();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_SYSTEM_SERVERSTATISTICS_H


