#include "TcpCommands/customers/removeCustomer.h"

#include "Logger/Logger.h"
#include "Config.h"

TcpCommand_customers_removeCustomer::TcpCommand_customers_removeCustomer() : TcpAbstractCommand()
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
QVariant TcpCommand_customers_removeCustomer::exec() {
    
    quint32 id;
    _streamRead(id);

    bool customerDataEditable = (Config::instance()->get("customers", "editable", "no") == "yes");
    if (customerDataEditable) {

        DatabasePool::get()->query("DELETE FROM customers WHERE id =" + QString::number(id));
        logWarning("Customer with id %1 removed", QString::number(id));
    }
    else {
        logNotice("Removing customers is not enabled!");
    }
    
    return true;
}


