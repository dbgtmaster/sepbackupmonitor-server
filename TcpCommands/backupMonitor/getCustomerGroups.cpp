#include "TcpCommands/backupMonitor/getCustomerGroups.h"

#include <Logger/Logger.h>

TcpCommand_backupMonitor_getCustomerGroups::TcpCommand_backupMonitor_getCustomerGroups() : TcpAbstractCommand()
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
QVariant TcpCommand_backupMonitor_getCustomerGroups::exec() {
    
    QStringList neededColumns;
    quint32 filterId;
    _streamRead(neededColumns);
    _streamRead(filterId);

    // Gültige Spalten:
    QStringList validColumns;
    validColumns << "id" << "name" << "auto_expand" << "description" << "position";

    // Würden gültige Spalten angegeben?
    foreach(QString col, neededColumns) {
        if (!validColumns.contains(col)) {
            logError("neededCols: Column '%1' is not allowed! Bye!", col);
            closeSocket();
            return false;
        }
    }

    QString sql = "SELECT " + neededColumns.join(", ") + " FROM backup_customer_groups";

    if (filterId > 0) {
        sql += QString(" WHERE id = %1").arg(QString::number(filterId));
    }

    QList< QHash<QString, QVariant> > returnList;
    DatabaseQuery q = _db->query(sql + "  ORDER by position");
    while (q.next()) {
        returnList.append(q.getAllColumns());
    }

    _streamWrite(returnList);

    return true;
}


