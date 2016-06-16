/**
  * Loescht eine Gruppe
  *
  * @incoming-stream:
  *     @ quint32 id		ID der Gruppe
  *
  * @outcoming-stream:
  *     @ none
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_CUSTOMERGROUPDELETE_H
#define TCPCOMMAND_BACKUPMONITOR_CUSTOMERGROUPDELETE_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_customerGroupDelete : public TcpAbstractCommand
{
public:
    
    TcpCommand_backupMonitor_customerGroupDelete();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_CUSTOMERGROUPDELETE_H



