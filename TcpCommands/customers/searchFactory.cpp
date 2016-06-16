#include "TcpCommands/customers/searchFactory.h"

#include "TcpCommands/customers/search.h"

TcpCommandFactory_customers_search::TcpCommandFactory_customers_search()
    : TcpAbstractCommandFactory("customers", "search")
{

}

TcpAbstractCommand* TcpCommandFactory_customers_search::createCommand() {

    return new TcpCommand_customers_search;
}

