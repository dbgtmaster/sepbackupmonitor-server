#include "TcpCommands/backupMonitor/getJobs.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Utils/UtilBackupMonitor.h"

TcpCommand_backupMonitor_getJobs::TcpCommand_backupMonitor_getJobs() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_backupMonitor_getJobs::exec() {

    QDate startDate;
    QDate endDate;
    quint32 customerId;
    QString jobFilter;
    QList<QDate> backwardDays;

    _streamRead(startDate);
    _streamRead(endDate);
    _streamRead(customerId);
    _streamRead(jobFilter);
    _streamRead(backwardDays);

    QString sql;
    sql = "SELECT j.id, j.customer_id, j.software_id, j.taskname, j.state, j.msg, j,drive_name, "
          "j.start_time, j.end_time, j.date, c.customer_name, j.type, j.group_softwareid, "
          "j.backup_software, "
          "(SELECT l.date FROM backup_jobs_log AS l WHERE l.customerid = j.customer_id AND l.taskname = j.taskname ORDER BY date DESC LIMIT 1) AS date_last_log_entry "
          "FROM backup_jobs AS j";
    sql += " LEFT JOIN customers c ON c.id = j.customer_id ";
    sql += " WHERE j.date >= '" + startDate.toString(Qt::ISODate) + "' AND j.date <= '" + endDate.toString(Qt::ISODate) + "'";
    if (customerId != -1) {
        sql += " AND j.customer_id = " + QString::number(customerId);
    }
    if (jobFilter != "*" && jobFilter != "") {
        jobFilter = jobFilter.replace("*", "%");
        sql += " AND LOWER(j.taskname) LIKE '"+ jobFilter.toLower() +"'";
    }
    sql += "ORDER BY j.date DESC, j.taskname";

    // SQL Basteln:
    DatabaseQuery q = _db->query(sql);
    QList<hashColumns> list;
    while (q.next()) {
        hashColumns cols = q.getAllColumns();

        /*
         * TODO: Job rückblickend..
        QList<QDate>::Iterator it;
        backwardList backward;
        for (it = backwardDays.begin(); it != backwardDays.end(); it++) {
            backward.append( QPair<QDate, quint32> (
                                 (*it), UtilBackupMonitor::stateOfBackupJob::get(
                                        customerId,
                                        q.row("taskname").toString(),
                                        ( (q.row("group_softwareid").toString() == "-1") ? UtilBackupMonitor::stateOfBackupJob::JOB : UtilBackupMonitor::stateOfBackupJob::GROUP ),
                                        q.row("date").toDate()
                                    )
                                )
                            );
        }
        cols["backward"].setValue<backwardList>(backward);
        */
        list.append( cols );
    }

    _streamWrite(list);

    return QVariant();
}

