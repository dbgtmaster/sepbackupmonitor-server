/**
  * Dieser Thread wird beim Dienststart mitgestartet.
  * Dieser cacht von allen Kunden eines Zeitraumes diverse Daten (Backup Status,..), damit diese nicht von der Datenbank abgefragt werden, wenn diese benötigt werden...
  */

#ifndef BACKUPMONITORDOCACHETHREAD_H
#define BACKUPMONITORDOCACHETHREAD_H

#include <QThread>

class BackupMonitorDoCacheThread : public QThread
{
    Q_OBJECT
public:
    explicit BackupMonitorDoCacheThread(QObject *parent = 0);
    
    void run();
signals:
    
public slots:
    
};

#endif // BACKUPMONITORDOCACHETHREAD_H
