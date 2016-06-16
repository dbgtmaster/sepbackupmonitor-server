#ifndef TCPCOMMANDFACTORY_CUSTOMERS_GETCUSTOMER_H
#define TCPCOMMANDFACTORY_CUSTOMERS_GETCUSTOMER_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_getCustomer : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_getCustomer();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_GETCUSTOMER_H

