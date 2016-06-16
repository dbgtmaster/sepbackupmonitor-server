#include "TcpCommands/customers/generateNewBackupMonitorKey.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

TcpCommand_customers_generateNewBackupMonitorKey::TcpCommand_customers_generateNewBackupMonitorKey() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_customers_generateNewBackupMonitorKey::exec() {

    quint32 customerId;
    _streamRead(customerId);

    QString random = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int randomSize = random.count();
    QString key;
    for (int i = 1; i <= 32; i++) {
        key += random.at( rand() % randomSize );
     }

    mapColumns update;
    update["backup_interface_key"] = key;
    _db->update(update, "customers", "id", customerId);

    _streamWrite(key);

    return QVariant();
}

