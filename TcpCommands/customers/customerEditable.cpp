#include "TcpCommands/customers/customerEditable.h"

#include "Config.h"

TcpCommand_customers_customerEditable::TcpCommand_customers_customerEditable() : TcpAbstractCommand()
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
QVariant TcpCommand_customers_customerEditable::exec() {
    
    _streamWrite((Config::instance()->get("customers", "editable", "no") == "yes"));
    return true;
}

