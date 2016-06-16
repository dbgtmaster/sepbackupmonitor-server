#include "TcpCommands/system/getAllSystemGroupsFactory.h"

#include "TcpCommands/system/getAllSystemGroups.h"

TcpCommandFactory_system_getAllSystemGroups::TcpCommandFactory_system_getAllSystemGroups()
    : TcpAbstractCommandFactory("system", "getAllSystemGroups")
{

}

TcpAbstractCommand* TcpCommandFactory_system_getAllSystemGroups::createCommand() {

    return new TcpCommand_system_getAllSystemGroups;
}

