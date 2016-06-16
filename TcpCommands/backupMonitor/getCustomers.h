/**
  * Gibt eine Liste mit allen Kunden zurück, welche den Backup-Monitor aktiviert haben.
  *
  * @incoming-stream:
  *     @ nothing
  *
  * @outcoming-stream:
  *     @ QList< QPair<quint32, QString> >:     Liste aller Kunden. quint32=Kundenid, QString=Kundenname
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERS_H
#define TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERS_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getCustomers : public TcpAbstractCommand
{
public:

    TcpCommand_backupMonitor_getCustomers();

    QVariant exec();

};

#endif // TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERS_H


