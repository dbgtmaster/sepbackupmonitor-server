#include "TcpCommands/customers/getCustomer.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"

TcpCommand_customers_getCustomer::TcpCommand_customers_getCustomer() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_customers_getCustomer::exec() {

    quint32 id;
    _streamRead(id);

    QStringList columns;
    _streamRead(columns);

    /**
      * Wurden gültige Spalten übergeben??
      */
    QStringList validCols;
    validCols << "id" << "customer_name" << "address" << "plz" << "city" << "backup_enabled"
              << "backup_interface_key" << "main_email" << "backup_prospective_jobs_mon"
              << "backup_prospective_jobs_tue" << "backup_prospective_jobs_wed" << "backup_prospective_jobs_thu"
              << "backup_prospective_jobs_fri" << "backup_prospective_jobs_sat" << "backup_prospective_jobs_sun"
              << "backup_last_hello" << "backup_sync_client_version" << "backup_group";
    foreach(QString col, columns) {
        if (!validCols.contains(col)) {
            logError("columns: Database column '%1' is not allowed! Bye!", col);
            closeSocket();
            return false;
        }
    }

    if (columns.count() == 0) {
        logError("Variable columns is empty! Bye!");
        closeSocket();
        return false;
    }

    // Sql an Datenbank senden:
    DatabaseQuery q = _db->query("SELECT " + columns.join(",") + " FROM customers WHERE id = "
                                 + QString::number( id ) );
    q.next();

    _streamWrite(q.getAllColumns());

    return true;
}

