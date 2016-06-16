/**
  * Liefert zu einem bestimmten Filter alle Ergebnisse zurück...
  *
  * @incoming-stream:
  *     @ QDate startDate       Ab welchem Datum sollen die Jobs gelistet werden.
  *     @ QDate toDate          Bis zu welchem Datum sollen die Jobs geliefert werden.
  *     @ quint32 customer      Von welchem Kunden sollen die Jobs gelistet werden (-1 => alle Kunden)
  *         // TODO @ QList<QDate> backwardDays  Eine Liste der Tage eines Jobs, welche Rückblickend der Status angezeigt werden soll?
  *                             Rückgabe im Format:
  *                             Row: backwardState, Value: QList< QPair<QDate, quint32 / state / > >
  *
  * @outcoming-stream:
  *     @ QList< QHash<QString, QVariant> >
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETJOBS_H
#define TCPCOMMAND_BACKUPMONITOR_GETJOBS_H

#include <TcpAbstractCommand.h>

typedef QList< QPair<QDate, quint32> > backwardList;

class TcpCommand_backupMonitor_getJobs : public TcpAbstractCommand
{
public:

    TcpCommand_backupMonitor_getJobs();

    QVariant exec();

};

//Q_DECLARE_METATYPE(backwardList)

#endif // TCPCOMMAND_BACKUPMONITOR_GETJOBS_H


