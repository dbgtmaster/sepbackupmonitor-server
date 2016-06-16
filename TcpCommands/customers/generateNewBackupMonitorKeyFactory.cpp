#include "TcpCommands/customers/generateNewBackupMonitorKeyFactory.h"

#include "TcpCommands/customers/generateNewBackupMonitorKey.h"

TcpCommandFactory_customers_generateNewBackupMonitorKey::TcpCommandFactory_customers_generateNewBackupMonitorKey()
    : TcpAbstractCommandFactory("customers", "generateNewBackupMonitorKey")
{

}

TcpAbstractCommand* TcpCommandFactory_customers_generateNewBackupMonitorKey::createCommand() {

    return new TcpCommand_customers_generateNewBackupMonitorKey;
}

