#ifndef TCPCOMMAND_LOGGEDINUSER_GETUSERCOLUMN_H
#define TCPCOMMAND_LOGGEDINUSER_GETUSERCOLUMN_H

#include <TcpAbstractCommand.h>

class TcpCommand_loggedInUser_getUserColumn : public TcpAbstractCommand
{
public:
    TcpCommand_loggedInUser_getUserColumn();

    QVariant exec();
};

#endif // TCPCOMMAND_LOGGEDINUSER_GETUSERCOLUMN_H
