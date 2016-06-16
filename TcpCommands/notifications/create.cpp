#include "TcpCommands/notifications/create.h"

TcpCommand_notifications_create::TcpCommand_notifications_create() : TcpAbstractCommand()
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
QVariant TcpCommand_notifications_create::exec() {
    
    QByteArray type, data;
    quint8 priority;

    _streamRead(type, data);
    _streamRead(priority);

    // Datensatz erstellen:
    DB db = DatabasePool::get();

    mapColumns insert;
    insert["type"] = type;
    insert["data"] = data;
    insert["priority"] = priority;
    db->insert(insert, "notifications");

    /**
      * @ TODO: Nun müssen wir noch alle Clients über diese neue Benachrichtigung benachrichtigen!
      */
    
    return true;
}


