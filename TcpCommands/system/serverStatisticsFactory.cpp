#include "TcpCommands/system/serverStatisticsFactory.h"

#include "TcpCommands/system/serverStatistics.h"

TcpCommandFactory_system_serverStatistics::TcpCommandFactory_system_serverStatistics()
    : TcpAbstractCommandFactory("system", "serverStatistics")
{
    
}

TcpAbstractCommand* TcpCommandFactory_system_serverStatistics::createCommand() {
    
    return new TcpCommand_system_serverStatistics;
}


