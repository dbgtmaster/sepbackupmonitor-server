#include "TcpCommands/administration/getUserGroupFactory.h"

#include "TcpCommands/administration/getUserGroup.h"

TcpCommandFactory_administration_getUserGroup::TcpCommandFactory_administration_getUserGroup()
    : TcpAbstractCommandFactory("administration", "getUserGroup")
{

}

TcpAbstractCommand* TcpCommandFactory_administration_getUserGroup::createCommand() {

    return new TcpCommand_administration_getUserGroup;
}

