#include "TcpCommands/backupMonitor/customerGroupDelete.h"

#include "Logger/Logger.h"

TcpCommand_backupMonitor_customerGroupDelete::TcpCommand_backupMonitor_customerGroupDelete() : TcpAbstractCommand()
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
QVariant TcpCommand_backupMonitor_customerGroupDelete::exec() {
    
    quint32 id;
    _streamRead(id);

    DB db = DatabasePool::get();

    db->query( QString("DELETE FROM backup_customer_groups WHERE id = %1").arg(QString::number(id)) );
    
    logNotice("BackupMonitor group with id '%1' deleted!", QString::number(id));

    /**
     * Wir sortieren die Position aller Gruppen neu.
     *
     * Sollte mal etwas mit der Sortierung schief gehen (2 Gruppen die gleiche POS), so kann man
     * ein neues Element anlegen, und wieder löschen, dann wird neu durchsortiert :-)..
     */
    quint32 pos = 1;
    DatabaseQuery result = db->query("SELECT id FROM backup_customer_groups ORDER BY position");
    while(result.next()) {
        db->query( QString("UPDATE backup_customer_groups SET position = %1 WHERE id = %2")
                   .arg( QString::number(pos), result.row("id").toString() ) );

        pos++;
    }

    return true;
}


