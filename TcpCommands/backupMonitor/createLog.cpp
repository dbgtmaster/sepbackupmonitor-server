#include "TcpCommands/backupMonitor/createLog.h"

#include <QDateTime>
#include "dbModels/dbUserModel.h"

TcpCommand_backupMonitor_createLog::TcpCommand_backupMonitor_createLog() : TcpAbstractCommand()
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
QVariant TcpCommand_backupMonitor_createLog::exec() {
    
    quint32 customerID;
    QString taskName;
    QString log;
    _streamRead(customerID);
    _streamRead(taskName);
    _streamRead(log);

    hashColumns insert;
    insert["customerid"] = customerID;
    insert["taskname"] = taskName;
    insert["log"] = log;
    insert["userid"] = _user->getColumn("id");
    insert["date"] = QDateTime::currentDateTime().toString();
    _db->insert(insert, "backup_jobs_log");

    return true;
}


