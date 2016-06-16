#include "TcpCommands/backupMonitor/getJob.h"

#include "Logger/Logger.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

TcpCommand_backupMonitor_getJob::TcpCommand_backupMonitor_getJob() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_backupMonitor_getJob::exec() {

    quint32 id;
    _streamRead(id);

    QStringList getCols;
    _streamRead(getCols);

    logNotice("Get data of job id '%1'", QString::number(id));

    QStringList validCols;
    validCols << "id" << "customer_id" << "software_id" << "taskname" << "state" << "msg" << "start_time"
              << "end_time" << "date" << "type" << "group_softwareid" << "backup_software" << "customer_name"
              << "source_list" << "exclude_list" << "throughput" << "data_size" << "job_type" << "drive_name" << "drive_type" << "drive_client_name";

    QStringList getColsRewrite;     // Vor den Spaltennamen müssen wir ein "j." oder "c." hängen (jobs / customers)
    foreach(QString col, getCols) {
        if (!validCols.contains(col)) {
            logError("columns: Database column '%1' is not allowed! Bye!", col);
            closeSocket();
            return false;
        }

        if (col == "customer_name") {
            getColsRewrite.append( "c." + col );
        }
        else {
            getColsRewrite.append( "j." + col );
        }
    }

    if (getCols.count() == 0) {
        logError("Variable columns is empty! Bye!");
        closeSocket();
        return false;
    }

    // SQL zusammenbauen:
    QString sql = "SELECT " + getColsRewrite.join(",") + " FROM backup_jobs AS j ";

    // Wird auch der customer_name ausgelesen, so müssen wir ein LEFT JOIN einbauen:
    if (getCols.contains("customer_name")) {
        sql += "LEFT JOIN customers c ON c.id = j.customer_id ";
    }

    sql += "WHERE j.id = " + QString::number( id ) ;

    // Sql an Datenbank senden:
    DatabaseQuery q = _db->query(sql);
    q.next();

    _streamWrite(q.getAllColumns());

    return true;
}

