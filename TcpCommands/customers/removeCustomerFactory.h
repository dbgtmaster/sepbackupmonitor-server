#ifndef TCPCOMMANDFACTORY_CUSTOMERS_REMOVECUSTOMER_H
#define TCPCOMMANDFACTORY_CUSTOMERS_REMOVECUSTOMER_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_removeCustomer : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_removeCustomer();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_REMOVECUSTOMER_H


