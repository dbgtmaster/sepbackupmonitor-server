#include "BackupMonitorDoCacheThread.h"

#include "Utils/UtilBackupMonitor.h"
#include "Logger/Logger.h"

#include "Database/Database.h"
#include "Database/DatabasePool.h"

#include "Config.h"
#include <QSet>

BackupMonitorDoCacheThread::BackupMonitorDoCacheThread(QObject *parent) :
    QThread(parent)
{
}

void BackupMonitorDoCacheThread::run() {

    while (true) {

        try {

            DB db = DatabasePool::get();


            logNotice("BackupMonitorDoCacheThread::run(): Start BackupMonitorDoCacheThread.");

            // Da unser Cache noch leer ist, können wir die Änderungen der Jobs ignorieren...
            db->query("UPDATE backup_jobs SET job_state_changed = false WHERE job_state_changed = true");

            /**
              * Im ersten Schritt schreiben wir den Backup- Status rückblickend der Kunden und der einzelnen Jobs in den Cache:
              */

            // Wir brauchen eine Liste aller Kunden, welche im Backupmonitor angezeigt werden:
            QList< QPair<quint32, QString> > customers = UtilBackupMonitor::getCustomers();

            // Wie viele Tage soll der Backup Status der Kunden gecacht werden?
            int stateBackwardDays = Config::instance()->get("cache", "backup_state_of_day_backward_days", "50").toInt();

            QList<QString> daysOfWeek;
            daysOfWeek << "sun" << "mon" << "tue" << "wed" << "thu" << "fri" << "sat" << "sun";

            DatabaseQuery q = db->query("SELECT id, customer_name, backup_prospective_jobs_mon, backup_prospective_jobs_tue, backup_prospective_jobs_wed, backup_prospective_jobs_thu, backup_prospective_jobs_fri, backup_prospective_jobs_sat, backup_prospective_jobs_sun from customers WHERE backup_enabled = true");
            while (q.next()) {

                quint32 customerId = q.row("id").toInt();
                QString customerName = q.row("customer_name").toString();

                logNotice("BackupMonitorDoCacheThread::run(): Start caching %1", customerName );

                // Hat der Kunde Backuptage definiert?
                if (q.row("backup_prospective_jobs_mon").toInt() < 1 &&
                        q.row("backup_prospective_jobs_tue").toInt() < 1 &&
                        q.row("backup_prospective_jobs_wed").toInt() < 1 &&
                        q.row("backup_prospective_jobs_thu").toInt() < 1 &&
                        q.row("backup_prospective_jobs_fri").toInt() < 1 &&
                        q.row("backup_prospective_jobs_sat").toInt() < 1 &&
                        q.row("backup_prospective_jobs_sun").toInt() < 1) {
                    logError("BackupMonitorDoCacheThread::run(): Customer '%1' have no backup days defined.", customerName );
                    continue;
                }

                QDate date(QDate::currentDate());
                for (int i = 1; i < stateBackwardDays; i++) {

                    // Ist dieser Tag ein Backuptag??
                    if (q.row("backup_prospective_jobs_" + daysOfWeek[date.dayOfWeek()] ).toInt() < 1 ) {
                        i--;
                        date = date.addDays(-1);
                        continue;
                    }

                    // Kundenstatus rückblickend:
                    UtilBackupMonitor::stateOfBackupDay::get(customerId, date, true);

                    // Die einzelnen Jobs des Kunden rückblickend:
                    DatabaseQuery q = db->query("SELECT taskname, group_softwareid, state FROM backup_jobs WHERE customer_id = "+ QString::number(customerId) +" AND date = '"+ date.toString(Qt::ISODate) +"'");
                    while(q.next()) {

                        // Tagesprotokoll braucht nicht gecacht werden, da dieser niemals abgefragt werden sollte..
                        if (q.row("state").toInt() == 50) continue;

                        /*
                         * @TODO: Für zukünftige relase...
                        // Jobstatus rückblickend cachen:
                        UtilBackupMonitor::stateOfBackupJob::get(
                                    customerId,
                                    q.row("taskname").toString(),
                                    ( (q.row("group_softwareid").toString() == "-1" ) ? UtilBackupMonitor::stateOfBackupJob::JOB : UtilBackupMonitor::stateOfBackupJob::GROUP  ),
                                    date,
                                    true);
                                    */
                    }

                    date = date.addDays(-1);
                }

                logNotice("BackupMonitorDoCacheThread::run(): Caching %1 finished", customerName );
            }

            /**
              * Nun überprüfn wir im Interval, ob sich Daten geändert haben...
              */
            while (true)  {

                // Eine Liste, bei welchen Kunden + Datum sich der Status der Jobs geändert hat...
                // QPair supported kein QDate, daher ein QDate*
                QSet< QPair<int /* customerid */, QDate*> > customersToRefreshCache;

                DatabaseQuery q = db->query("SELECT id, date, customer_id, taskname, group_softwareid FROM backup_jobs WHERE job_state_changed = true");
                while (q.next()) {

                    // Den Status des Jobs aktualisieren:
                    UtilBackupMonitor::stateOfBackupJob::get(
                                q.row("customer_id").toInt(),
                                q.row("taskname").toString(),
                                ( (q.row("group_softwareid").toString() == "-1" ) ? UtilBackupMonitor::stateOfBackupJob::JOB : UtilBackupMonitor::stateOfBackupJob::GROUP  ),
                                q.row("date").toDate(),
                                true
                                );

                    customersToRefreshCache.insert( QPair<int, QDate*>(q.row("customer_id").toInt(), new QDate(q.row("date").toDate())) );

                    db->query("UPDATE backup_jobs SET job_state_changed = false WHERE id = " + q.row("id").toString() + " ");
                }

                // Nun die Kunden der geänderten Jobs aktualisieren:
                QSet< QPair<int /* customerid */, QDate*> >::Iterator it;
                for (it = customersToRefreshCache.begin(); it != customersToRefreshCache.end(); it++) {

                    QPair<int, QDate*> pair = (*it);

                    UtilBackupMonitor::stateOfBackupDay::get(pair.first, *pair.second, true);
                    delete pair.second;
                }

                logNotice("BackupMonitorDoCacheThread::run(): sleep 10 seconds.");
                sleep(10);

            }
        }
        catch (SqlException &e) {

            logFatal("SQL Error in thread BackupMonitorDoCacheThread!");
            logFatal("SQL: \n%1\n%2", e.sql(), e.error().databaseText());
            logFatal("Driver error: %1", e.error().driverText() );

            logFatal("Try again in 10 seconds...");
            sleep(10);
        }
   }
}
