#include "UtilBackupMonitor.h"

#include "Database/DatabasePool.h"
#include "Config.h"
#include <QDate>

#include <QReadWriteLock>

#include "Logger/Logger.h"


// Cache & Mutex für den Kundencache:
QReadWriteLock UtilBackupMonitor::stateOfBackupDay::_cacheMutex;
QHash<quint32 , QHash< QByteArray, UtilBackupMonitor::stateOfBackupDay::backupState > > UtilBackupMonitor::stateOfBackupDay::_cache;

int UtilBackupMonitor::stateOfBackupDay::_cacheMaxEntries = Config::instance()->get("cache", "backup_state_of_day_customers_max_entries", "10000").toInt();
int UtilBackupMonitor::stateOfBackupDay::_entries = 0;

// Cache & Mutex für den Jobcache:
QReadWriteLock UtilBackupMonitor::stateOfBackupJob::_cacheMutex;
QHash<quint32, QHash< QByteArray, QHash<QString, quint32 > > > UtilBackupMonitor::stateOfBackupJob::_cache;

UtilBackupMonitor::stateOfBackupDay::backupState UtilBackupMonitor::stateOfBackupDay::get(quint32 customerId, const QDate &date, bool refreshCache) {

    QByteArray dateString = date.toString("yyyyMMdd").toUtf8();

    /**
     * Existiert die angegebene Kundenid + Datum schon im Cache?
     */
    if (!refreshCache) {

        QReadLocker mutexRead(&_cacheMutex);
        if (_cache.contains(customerId)) {
            // Existiert das Datum dazu?
            QHash<QByteArray, backupState> states = _cache.value(customerId);
            if (states.contains(dateString)) {
                logDebug("UtilBackupMonitor::stateOfBackupDay::get(): get value by cache");
                return states.value(dateString);
            }
        }
        else {
            // Wir legen die Struktur im Cache an...
            mutexRead.unlock();
            QWriteLocker mutexWrite(&_cacheMutex);
            _cache[customerId] = QHash<QByteArray, backupState>();
            mutexWrite.unlock();
        }
        mutexRead.unlock();
    }

    /**
     * Der angebene Kunden + Datum existiert noch nicht im Cache, wir lesen aus der Datenbank aus...
     */

    backupState dayState;

    DB db = DatabasePool::get();
    DatabaseQuery query = db->query("SELECT state, type, taskname FROM backup_jobs WHERE date = '"+ date.toString(Qt::ISODate) +"' AND customer_id = "+ QString::number(customerId) + "");
    while(query.next()) {

        dayState.total++;

        if (query.row("taskname").toString() == "Tagesprotokoll") continue;
        if (query.row("taskname").toString() == "sm_newday") continue;
        if (query.row("type").toInt() == 1) continue;   // Es ist eine Gruppe

        if (query.row("state").toInt() == 0) {
            dayState.successful++;
        }
        else if (query.row("state").toInt() == 1) {
            dayState.warning++;
        }
        else if (query.row("state").toInt() == 80) {
            dayState.running++;
        }
        else if (query.row("state").toInt() == 6) {
            dayState.running++;
        }
        else if (query.row("state").toInt() == 50) {
            // Tagesprotokoll wird ignoriert...
        }
        else if (query.row("state").toInt() == 3) {
            dayState.aborted++;
        }
        else {
            dayState.error++;
        }

        if (!query.row("taskname").toString().startsWith("INIT_")) {
            dayState.totalWithoutMainJobs++;
        }
    }

    logDebug( QString("UtilBackupMonitor::getStateOfBackupDay(): State of customerid %2, date %1: total: %3, totalWithoutMainJobs: %4, %5 successful, %6 warnings, %7 errors, %8 aborted, %9 running")
               .arg(date.toString(), QString::number(customerId), QString::number(dayState.total), QString::number(dayState.totalWithoutMainJobs), QString::number(dayState.successful), QString::number(dayState.warning), QString::number(dayState.error), QString::number(dayState.aborted), QString::number(dayState.running) ));

    logDebug("UtilBackupMonitor::stateOfBackupDay::get(): get value NOT from cache");

    // Status des Tages in den Cache schreiben:
    QWriteLocker mutexWrite(&_cacheMutex);

    _cache[customerId].insert(dateString, dayState);
    _entries++;
    if (_entries > _cacheMaxEntries) {
        // Wenn das Limit erreicht worden ist, löschen wir einfach den Cache, da wir derzeit keine Möglichkeit haben,
        // den ältersten Eintrag zu löschen.
        _cache.clear();
        _entries = 0;
    }

    mutexWrite.unlock();

    return dayState;
}

quint32 UtilBackupMonitor::stateOfBackupJob::get(quint32 customerId, const QString &jobName, jobType type, const QDate &date, bool refreshCache) {

    QByteArray dateString = date.toString("yyyyMMdd").toUtf8();
    QString jobNameCache = (type == JOB) ? "j_" + jobName : "g_" + jobName;

    /**
     * Existiert die angegebene Kundenid + Datum + Jobname schon im Cache?
     */
    if (!refreshCache) {

        QReadLocker mutexRead(&_cacheMutex);
        if (_cache.contains(customerId)) {
            // Existiert das Datum dazu?
            QHash<QByteArray, QHash<QString, quint32> > hashCustomers = _cache.value(customerId);
            if (hashCustomers.contains(dateString)) {

                // Existiert der Jobname dazu dazu?
                QHash<QString, quint32> hashDates = hashCustomers.value(dateString);

                if (hashDates.contains(jobNameCache)) {

                    logDebug("UtilBackupMonitor::stateOfBackupJob::get(): get value by cache");
                    return hashDates.value(jobNameCache);
                }
            }
            else {
                // Wir legen die Struktur im Cache an...
                mutexRead.unlock();
                QWriteLocker mutexWrite(&_cacheMutex);
                hashCustomers[dateString] = QHash<QString, quint32 >();
                mutexWrite.unlock();
            }
        }
        else {
            // Wir legen die Struktur im Cache an...
            mutexRead.unlock();
            QWriteLocker mutexWrite(&_cacheMutex);
            _cache[customerId] = QHash< QByteArray, QHash<QString, quint32 > >();
            mutexWrite.unlock();
        }
        mutexRead.unlock();
    }

    /**
     * Der angebene Kunden + Datum + Jobname existiert noch nicht im Cache, wir lesen aus der Datenbank aus...
     */

    logDebug("UtilBackupMonitor::stateOfBackupJob::get(): get value NOT from cache");

    qDebug() << "aaaaa1";
    DB db = DatabasePool::get();
    qDebug() << "aaaaa2";
    quint32 state;
    DatabaseQuery query = db->query("SELECT state FROM backup_jobs WHERE date = '"+ date.toString(Qt::ISODate) +"' AND customer_id = "+ QString::number(customerId) + " AND taskname = "+ db->escape(jobName) +" AND group_softwareid "+ ( (type == JOB) ? " = '-1'" : " != '-1'"  ) +"");
    int count = query.count();
    if (count == 1) {

        state = query.row("state").toInt();
    }
    else if (count == 0) {
        // Jobname existiert nicht.
        state = 100;     // NOT_AVAILABLE
    }
    else {
        state = 99;
        /**
         * @TODO:
         * 1.) Den Drivenamen im jobCacheName mit einbeziehen.
         * 2.) Wenn mehrere Jobs zurückgeliefert werden, soll der schlechterste Status genommen werden!
         */
        logDebug("Problem with a job! select backup job returns more entries! rows: " + QString::number(count));
    }

    logDebug( QString("UtilBackupMonitor::getStateOfBackupDay(): State of customerid %1, date %2: jobName: %3, JobOrGroup: %4, state: %5")
              .arg(QString::number(customerId), date.toString(), jobName, ( (type == JOB) ? "job" : "group" )  ));

    // Status des Tages in den Cache schreiben:
    QWriteLocker mutexWrite(&_cacheMutex);
    _cache[customerId][dateString].insert(jobNameCache, state);
    mutexWrite.unlock();

    return state;
}

QList< QPair<quint32, QString> > UtilBackupMonitor::getCustomers() {

    QList< QPair<quint32, QString> > list;

    DatabaseQuery q = DatabasePool::get()->query("SELECT customer_name, id FROM customers WHERE backup_enabled = true ORDER BY customer_name");
    while (q.next()) {
        list << QPair<quint32, QString>( q.row("id").toInt(), q.row("customer_name").toString() );
    }

    return list;
}
