#include "TcpCommands/customers/customerEditableFactory.h"

#include "TcpCommands/customers/customerEditable.h"

TcpCommandFactory_customers_customerEditable::TcpCommandFactory_customers_customerEditable()
    : TcpAbstractCommandFactory("customers", "customerEditable")
{
    
}

TcpAbstractCommand* TcpCommandFactory_customers_customerEditable::createCommand() {
    
    return new TcpCommand_customers_customerEditable;
}


