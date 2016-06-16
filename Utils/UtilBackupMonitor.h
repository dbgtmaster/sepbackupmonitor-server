#ifndef UTILBACKUPMONITOR_H
#define UTILBACKUPMONITOR_H

#include <QtCore>
#include <QHash>
#include <QMutex>
#include <QDate>
#include <QList>
#include <QPair>

namespace UtilBackupMonitor {

// Liefert von einem bestimmten Backup Tag den Status, und cacht die Ergebnisse zwischen...
class stateOfBackupDay {
public:
    struct backupState {
        quint32 total, totalWithoutMainJobs, successful, warning, error, aborted, running;

        backupState() : total(0), totalWithoutMainJobs(0), successful(0), warning(0), error(0), aborted(0), running(0) {

        }
    };

private:

    static QReadWriteLock _cacheMutex;
    static QHash<quint32 /* customerid */, QHash< QByteArray /* date */, backupState > > _cache;

    static int _cacheMaxEntries;
    static int _entries;

public:
    // Wenn "bool refreshCache" true, dann wird der Wert von der Datenbank neu ausgelesen und im Cache geschrieben:
    static backupState get(quint32 customerId, const QDate &date, bool refreshCache = false);
    static inline int getMaxCacheEntries() {
        return _cacheMaxEntries;
    }
    static inline int getCacheEntries() {
        QReadLocker mutexRead(&_cacheMutex);
        return _entries;
    }
};



// Liefert von einem bestimmten Job den Status, und cacht die Ergebnisse zwischen...
class stateOfBackupJob {
private:

    static QReadWriteLock _cacheMutex;
    static QHash<quint32 /* customerid */, QHash< QByteArray /* date */, QHash<QString /* jobname */, quint32 /* state of job */ > > > _cache;

public:

    enum jobType {
        JOB,
        GROUP
    };

    // JobType: Ob es sich um einen Job, oder einer Gruppe handelt.
    // Wenn "bool refreshCache" true, dann wird der Wert von der Datenbank neu ausgelesen und im Cache geschrieben:
    static quint32 get(quint32 customerId, const QString &jobName, jobType type, const QDate &date, bool refreshCache = false);
};



// Liefert eine Liste mit allen Kunden (id->name), welche den Backupmonitor aktiviert haben...
QList< QPair<quint32, QString> > getCustomers();

}

#endif // UTILBACKUPMONITOR_H
