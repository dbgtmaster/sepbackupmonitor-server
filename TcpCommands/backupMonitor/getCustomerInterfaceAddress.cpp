#include "TcpCommands/backupMonitor/getCustomerInterfaceAddress.h"

#include "Config.h"

TcpCommand_backupMonitor_getCustomerInterfaceAddress::TcpCommand_backupMonitor_getCustomerInterfaceAddress() : TcpAbstractCommand()
{
}

/**
  * Fhrt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead(var)
  * Schreiben: dataStreamWrite(var);
  * Lesen & Schreiben werfen eine Exception, welche vom TcpThread aufgefangen wird, wenn ungltige Daten gelesen / geschrieben werden...
  * Auf Datastream direkt zugreifen (zB. um zu berprfen ob noch Daten vorhanden): _dataStreamRead, _dataStreamWrite
  * Socket close: _readSocket->close();
  *
  * @return QVariant: Es kann ein beliebiger Wert zurckgegeben werden... Wenn void postExec(const QVariant &var)  berladen wird,
  *						kann auf diesen Wert wieder zugegriffen werden.
  */
QVariant TcpCommand_backupMonitor_getCustomerInterfaceAddress::exec() {
    
    _streamWrite( Config::instance()->get("backupmonitor", "customer_interface_address") );
    return true;

    return true;
}


