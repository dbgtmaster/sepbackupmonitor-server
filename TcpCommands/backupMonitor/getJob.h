/**
  * Gibt Informationen zu einen einzelnen Job zurück...
  *
  * @incoming-stream:
  *     @ quint32           ID des Jobs
  *     @ QStringList       Spalten, welche gelesen werden sollen...
  *
  * @outcoming-stream:
  *     @ QHash<QString, QVariant>      Zurückliefernde Datensätze
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETJOB_H
#define TCPCOMMAND_BACKUPMONITOR_GETJOB_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getJob : public TcpAbstractCommand
{
public:

    TcpCommand_backupMonitor_getJob();

    QVariant exec();

};

#endif // TCPCOMMAND_BACKUPMONITOR_GETJOB_H


