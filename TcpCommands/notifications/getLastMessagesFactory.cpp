#include "TcpCommands/notifications/getLastMessagesFactory.h"

#include "TcpCommands/notifications/getLastMessages.h"

TcpCommandFactory_notifications_getLastMessages::TcpCommandFactory_notifications_getLastMessages()
    : TcpAbstractCommandFactory("notifications", "getLastMessages")
{
    
}

TcpAbstractCommand* TcpCommandFactory_notifications_getLastMessages::createCommand() {
    
    return new TcpCommand_notifications_getLastMessages;
}


