#include "TcpCommands/loggedInUser/isInSystemGroupFactory.h"

#include "TcpCommands/loggedInUser/isInSystemGroup.h"

TcpCommand_loggedInUser_isInSystemGroupFactory::TcpCommand_loggedInUser_isInSystemGroupFactory()
    : TcpAbstractCommandFactory("loggedInUser", "isInSystemGroup")
{

}

TcpAbstractCommand* TcpCommand_loggedInUser_isInSystemGroupFactory::createCommand() {

    return new TcpCommand_loggedInUser_isInSystemGroup;
}

