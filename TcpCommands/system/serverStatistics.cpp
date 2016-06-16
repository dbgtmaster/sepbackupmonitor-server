#include "TcpCommands/system/serverStatistics.h"

#include "Utils/UtilBackupMonitor.h"

TcpCommand_system_serverStatistics::TcpCommand_system_serverStatistics() : TcpAbstractCommand()
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
QVariant TcpCommand_system_serverStatistics::exec() {
    
    QHash<QString, QVariant> data;

    data.insert("databasepool_min", DatabasePool::getMinConnections());
    data.insert("databasepool_max", DatabasePool::getMaxConnections());
    data.insert("databasepool_free", DatabasePool::getFreeConnections());
    data.insert("databasepool_total_managed", DatabasePool::getTotalManagedConnections());
    data.insert("cache_backup_state_of_day_customers_entries", UtilBackupMonitor::stateOfBackupDay::getCacheEntries());
    data.insert("cache_backup_state_of_day_customers_max_entries", UtilBackupMonitor::stateOfBackupDay::getMaxCacheEntries());
    qDebug() << data;
    _streamWrite(data);

    return true;
}


