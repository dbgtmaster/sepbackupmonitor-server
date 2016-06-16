#include "TcpCommands/backupMonitor/getJobProtocolFactory.h"

#include "TcpCommands/backupMonitor/getJobProtocol.h"

TcpCommandFactory_backupMonitor_getJobProtocol::TcpCommandFactory_backupMonitor_getJobProtocol()
    : TcpAbstractCommandFactory("backupMonitor", "getJobProtocol")
{

}

TcpAbstractCommand* TcpCommandFactory_backupMonitor_getJobProtocol::createCommand() {

    return new TcpCommand_backupMonitor_getJobProtocol;
}

