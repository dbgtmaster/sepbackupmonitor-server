#include "TcpCommands/backupMonitor/getLog.h"

TcpCommand_backupMonitor_getLog::TcpCommand_backupMonitor_getLog() : TcpAbstractCommand()
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
QVariant TcpCommand_backupMonitor_getLog::exec() {

    quint32 customerID;
    _streamRead(customerID);

    QString taskName;
    _streamRead(taskName);

    // Create query:
    QString sql = "SELECT l.*, u.username FROM backup_jobs_log AS l "
                    "LEFT JOIN users u ON u.id = l.userid "
                  "WHERE l.customerid = " + QString::number(customerID);

    if (!taskName.isEmpty()) {
        sql += " AND TaskName = '" + taskName + "'";
    }

    sql += " ORDER BY date DESC";

    DatabaseQuery r = _db->query(sql);

    _streamWrite(r.fetchAll());

    return true;
}
