#include "TcpCommands/notifications/getLastMessages.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

TcpCommand_notifications_getLastMessages::TcpCommand_notifications_getLastMessages() : TcpAbstractCommand()
{
}

/**
  * Fhrt den eigentlichen Tcp- Befehl aus...
  * Lesen: _streamRead(var)
  * Schreiben: _streamWrite(var);
  * Lesen & Schreiben werfen eine Exception, welche vom TcpThread aufgefangen wird, wenn ungltige Daten gelesen / geschrieben werden...
  * Auf Datastream direkt zugreifen (zB. um zu berprfen ob noch Daten vorhanden): _dataStreamRead, _dataStreamWrite
  * Socket close: _readSocket->close();
  *
  * @return QVariant: Es kann ein beliebiger Wert zurckgegeben werden... Wenn void postExec(const QVariant &var)  berladen wird,
  *						kann auf diesen Wert wieder zugegriffen werden.
  */
QVariant TcpCommand_notifications_getLastMessages::exec() {
    
    DB db = DatabasePool::get();
    QList<hashColumns> data = db->query("SELECT type, data, priority, date FROM notifications ORDER BY id LIMIT 30").fetchAll();

    _streamWrite(data);

    return true;
}


