#ifndef TCPCOMMANDFACTORY_CUSTOMERS_MODCUSTOMER_H
#define TCPCOMMANDFACTORY_CUSTOMERS_MODCUSTOMER_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_modCustomer : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_modCustomer();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_MODCUSTOMER_H

