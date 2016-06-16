#include "userLoginFactory.h"

#include "TcpCommands/main/userLogin.h"

TcpCommandFactory_main_userLogin::TcpCommandFactory_main_userLogin()
    : TcpAbstractCommandFactory("main", "userLogin")
{

}

TcpAbstractCommand* TcpCommandFactory_main_userLogin::createCommand() {

    return new TcpCommand_main_userLogin;
}
