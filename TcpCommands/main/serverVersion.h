#ifndef TCPCOMMAND_MAIN_SERVERVERSION_H
#define TCPCOMMAND_MAIN_SERVERVERSION_H

#include <TcpAbstractCommand.h>

class TcpCommand_main_serverVersion : public TcpAbstractCommand
{
public:
    TcpCommand_main_serverVersion();

    QVariant exec();
};

#endif // TCPCOMMAND_MAIN_SERVERVERSION_H
