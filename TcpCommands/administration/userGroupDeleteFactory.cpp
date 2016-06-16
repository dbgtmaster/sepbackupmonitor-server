#include "TcpCommands/administration/userGroupDeleteFactory.h"

#include "TcpCommands/administration/userGroupDelete.h"

TcpCommandFactory_administration_userGroupDelete::TcpCommandFactory_administration_userGroupDelete()
    : TcpAbstractCommandFactory("administration", "userGroupDelete")
{

}

TcpAbstractCommand* TcpCommandFactory_administration_userGroupDelete::createCommand() {

    return new TcpCommand_administration_userGroupDelete;
}

