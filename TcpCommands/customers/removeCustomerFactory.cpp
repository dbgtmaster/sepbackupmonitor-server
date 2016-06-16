#include "TcpCommands/customers/removeCustomerFactory.h"

#include "TcpCommands/customers/removeCustomer.h"

TcpCommandFactory_customers_removeCustomer::TcpCommandFactory_customers_removeCustomer()
    : TcpAbstractCommandFactory("customers", "removeCustomer")
{
    
}

TcpAbstractCommand* TcpCommandFactory_customers_removeCustomer::createCommand() {
    
    return new TcpCommand_customers_removeCustomer;
}


