#ifndef CONNECTTCPFACTORY_H
#define CONNECTTCPFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_main_connectTcp : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_main_connectTcp();

    TcpAbstractCommand* createCommand();
};

#endif // CONNECTTCPFACTORY_H
