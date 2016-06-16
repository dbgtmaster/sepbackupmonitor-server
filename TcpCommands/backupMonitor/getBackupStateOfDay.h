/**
  * Liefert von Status von einer Liste an Backuptagen
  * ACHTUNG: Es werden maximal 100 Backup- Tage zurückgeliefert!!
  *
  * @incoming-stream:
  *     @ QList<QDate> dates		Eine Liste, von welchen Datums der Status ermittelt werden soll.
  *     @ quint32 customerId        ID des Kunden
  *
  * @outcoming-stream:
  *
  *     QList< QHash<QByteArray, QVariant> >        Daten eines Backup- Tages
  *             QHash beinhaltet:
  *                 @ date                  -> QDate        Datum des Backup- Tages
  *                 @ total                 -> quint32      Wie viele Jobs sind insgesamt gelaufen
  *                 @ totalWithoutMainJobs  -> quint32      Wie viele Jobs sind gelaufen, ausgeschlossen von Jobs folgenden Jobs: 'INIT_*', 'sm_newday', Tagesprotokoll
  *                 @ successful            -> quint32      Wie viele Jobs erfolgreich
  *                 @ warning               -> quint32      Wie viele mit Warnungen
  *                 @ error                 -> quint32      Wie viele Jobs fehlgeschlagen
  *                 @ aborted               -> quint32      Wie viele Jobs abgebrochen
  *                 @ running               -> quint32      Wie viele Jobs noch am laufen
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETBACKUPSTATEOFDAY_H
#define TCPCOMMAND_BACKUPMONITOR_GETBACKUPSTATEOFDAY_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getBackupStateOfDay : public TcpAbstractCommand
{
public:
    
    TcpCommand_backupMonitor_getBackupStateOfDay();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_GETBACKUPSTATEOFDAY_H



