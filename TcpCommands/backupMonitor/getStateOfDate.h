/**
  * Gibt von einem bestimmten Backup- Tag den Gesamtstatus der Sicherungen zurueck.. (= wie viele erfolgreich, warnungen,fehlerhaft,laufend)
  *
  * @incoming-stream:
  *     @ nothing
  *
  * @outcoming-stream:
  *     @ nothing
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETSTATEOFDATE_H
#define TCPCOMMAND_BACKUPMONITOR_GETSTATEOFDATE_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getStateOfDate : public TcpAbstractCommand
{
public:

    TcpCommand_backupMonitor_getStateOfDate();

    QVariant exec();

};

#endif // TCPCOMMAND_BACKUPMONITOR_GETSTATEOFDATE_H


