#include "TcpCommands/loggedInUser/hasTcpPermissionFactory.h"

#include "TcpCommands/loggedInUser/hasTcpPermission.h"

TcpCommandFactory_loggedInUser_hasTcpPermission::TcpCommandFactory_loggedInUser_hasTcpPermission()
    : TcpAbstractCommandFactory("loggedInUser", "hasTcpPermission")
{
    
}

TcpAbstractCommand* TcpCommandFactory_loggedInUser_hasTcpPermission::createCommand() {
    
    return new TcpCommand_loggedInUser_hasTcpPermission;
}


