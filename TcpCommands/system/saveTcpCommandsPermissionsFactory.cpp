#include "TcpCommands/system/saveTcpCommandsPermissionsFactory.h"

#include "TcpCommands/system/saveTcpCommandsPermissions.h"

TcpCommandFactory_system_saveTcpCommandsPermissions::TcpCommandFactory_system_saveTcpCommandsPermissions()
    : TcpAbstractCommandFactory("system", "saveTcpCommandsPermissions")
{

}

TcpAbstractCommand* TcpCommandFactory_system_saveTcpCommandsPermissions::createCommand() {

    return new TcpCommand_system_saveTcpCommandsPermissions;
}

