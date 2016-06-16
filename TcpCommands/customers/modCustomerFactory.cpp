#include "TcpCommands/customers/modCustomerFactory.h"

#include "TcpCommands/customers/modCustomer.h"

TcpCommandFactory_customers_modCustomer::TcpCommandFactory_customers_modCustomer()
    : TcpAbstractCommandFactory("customers", "modCustomer")
{

}

TcpAbstractCommand* TcpCommandFactory_customers_modCustomer::createCommand() {

    return new TcpCommand_customers_modCustomer;
}

