#include "TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.h"

#include "TcpCommands/backupMonitor/getCustomerInterfaceAddress.h"

TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress::TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress()
    : TcpAbstractCommandFactory("backupMonitor", "getCustomerInterfaceAddress")
{
    
}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress::createCommand() {
    
    return new TcpCommand_backupMonitor_getCustomerInterfaceAddress;
}


