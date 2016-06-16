#include "TcpCommands/administration/userGroupModFactory.h"

#include "TcpCommands/administration/userGroupMod.h"

TcpCommandFactory_administration_userGroupMod::TcpCommandFactory_administration_userGroupMod()
    : TcpAbstractCommandFactory("administration", "userGroupMod")
{

}

TcpAbstractCommand* TcpCommandFactory_administration_userGroupMod::createCommand() {

    return new TcpCommand_administration_userGroupMod;
}

