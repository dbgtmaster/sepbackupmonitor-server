#ifndef TCPCOMMANDFACTORY_CUSTOMERS_CUSTOMEREDITABLE_H
#define TCPCOMMANDFACTORY_CUSTOMERS_CUSTOMEREDITABLE_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_customerEditable : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_customerEditable();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_CUSTOMEREDITABLE_H


