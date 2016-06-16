#include "TcpCommands/notifications/createFactory.h"

#include "TcpCommands/notifications/create.h"

TcpCommandFactory_notifications_create::TcpCommandFactory_notifications_create()
    : TcpAbstractCommandFactory("notifications", "create")
{
    
}

TcpAbstractCommand* TcpCommandFactory_notifications_create::createCommand() {
    
    return new TcpCommand_notifications_create;
}


