#include "TcpCommands/administration/getAllUserGroupsFactory.h"

#include "TcpCommands/administration/getAllUserGroups.h"

TcpCommandFactory_administration_getAllUserGroups::TcpCommandFactory_administration_getAllUserGroups()
    : TcpAbstractCommandFactory("administration", "getAllUserGroups")
{

}

TcpAbstractCommand* TcpCommandFactory_administration_getAllUserGroups::createCommand() {

    return new TcpCommand_administration_getAllUserGroups;
}

