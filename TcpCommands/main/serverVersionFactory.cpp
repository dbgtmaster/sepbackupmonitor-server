#include "serverVersionFactory.h"

#include "TcpCommands/main/serverVersion.h"

TcpCommandFactory_main_serverVersion::TcpCommandFactory_main_serverVersion()
    : TcpAbstractCommandFactory("main", "serverVersion")
{

}

TcpAbstractCommand* TcpCommandFactory_main_serverVersion::createCommand() {

    return new TcpCommand_main_serverVersion;
}
