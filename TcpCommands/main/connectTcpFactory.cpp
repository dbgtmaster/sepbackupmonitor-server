#include "connectTcpFactory.h"

#include "TcpCommands/main/connectTcp.h"

TcpCommandFactory_main_connectTcp::TcpCommandFactory_main_connectTcp()
    : TcpAbstractCommandFactory("main", "connectTcp")
{

}

TcpAbstractCommand* TcpCommandFactory_main_connectTcp::createCommand() {

    return new TcpCommand_main_connectTcp;
}
