#ifndef CONNECTTCP_H
#define CONNECTTCP_H

#include <TcpAbstractCommand.h>

class TcpCommand_main_connectTcp : public TcpAbstractCommand
{
public:
    TcpCommand_main_connectTcp();

    QVariant exec();
};

#endif // CONNECTTCP_H
