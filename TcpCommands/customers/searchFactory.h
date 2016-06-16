#ifndef TCPCOMMANDFACTORY_CUSTOMERS_SEARCH_H
#define TCPCOMMANDFACTORY_CUSTOMERS_SEARCH_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_search : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_search();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_SEARCH_H

