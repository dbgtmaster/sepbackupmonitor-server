#ifndef TCPCOMMAND_MAIN_USERLOGIN_H
#define TCPCOMMAND_MAIN_USERLOGIN_H

#include <TcpAbstractCommand.h>

class TcpCommand_main_userLogin : public TcpAbstractCommand
{

private:

public:
    TcpCommand_main_userLogin();

    QVariant exec();
    void postExec(const QVariant &var);
};

#endif // TCPCOMMAND_MAIN_USERLOGIN_H
