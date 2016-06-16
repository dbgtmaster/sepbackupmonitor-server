#include "TcpCommands/system/getAllTcpCommandsFactory.h"

#include "TcpCommands/system/getAllTcpCommands.h"

TcpCommandFactory_system_getAllTcpCommands::TcpCommandFactory_system_getAllTcpCommands()
    : TcpAbstractCommandFactory("system", "getAllTcpCommands")
{

}

TcpAbstractCommand* TcpCommandFactory_system_getAllTcpCommands::createCommand() {

    return new TcpCommand_system_getAllTcpCommands;
}

