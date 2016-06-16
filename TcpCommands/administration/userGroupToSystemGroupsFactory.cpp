#include "TcpCommands/administration/userGroupToSystemGroupsFactory.h"

#include "TcpCommands/administration/userGroupToSystemGroups.h"

TcpCommandFactory_administration_userGroupToSystemGroups::TcpCommandFactory_administration_userGroupToSystemGroups()
    : TcpAbstractCommandFactory("administration", "userGroupToSystemGroups")
{

}

TcpAbstractCommand* TcpCommandFactory_administration_userGroupToSystemGroups::createCommand() {

    return new TcpCommand_administration_userGroupToSystemGroups;
}

