/**
  * Liefert eine Liste mit allen verfügbaren Kunden- Gruppen zurück...
  *
  * @incoming-stream:
  *     @ QStringList        Eine Liste mit Parameter, welche benötigt werden.
  *     @ quint32 filterId   Wenn nach einer bestimmten ID gefiltert werden soll, ansonsten 0.
  *
  * @outcoming-stream:
  *     @ QList< QHash<QString key, QVariant value >>        Eine Liste mit allen Gruppen, im QHash deren benötigten Parameter
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERGROUPS_H
#define TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERGROUPS_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getCustomerGroups : public TcpAbstractCommand
{
public:
    
    TcpCommand_backupMonitor_getCustomerGroups();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERGROUPS_H



