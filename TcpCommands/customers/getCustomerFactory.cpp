#include "TcpCommands/customers/getCustomerFactory.h"

#include "TcpCommands/customers/getCustomer.h"

TcpCommandFactory_customers_getCustomer::TcpCommandFactory_customers_getCustomer()
    : TcpAbstractCommandFactory("customers", "getCustomer")
{

}

TcpAbstractCommand* TcpCommandFactory_customers_getCustomer::createCommand() {

    return new TcpCommand_customers_getCustomer;
}

