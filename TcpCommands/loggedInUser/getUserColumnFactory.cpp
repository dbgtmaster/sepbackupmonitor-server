#include "getUserColumnFactory.h"

#include "TcpCommands/loggedInUser/getUserColumn.h"

TcpCommandFactory_loggedInUser_getUserColumn::TcpCommandFactory_loggedInUser_getUserColumn()
    : TcpAbstractCommandFactory("loggedInUser", "getUserColumn")
{

}

TcpAbstractCommand* TcpCommandFactory_loggedInUser_getUserColumn::createCommand() {

    return new TcpCommand_loggedInUser_getUserColumn;
}
