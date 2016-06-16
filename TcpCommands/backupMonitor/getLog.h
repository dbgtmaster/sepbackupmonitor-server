/**
  * Gibt das Log von einem Kunden bzw. Job aus.
  *
  * @incoming-stream:
  *     @ none
  *
  * @outcoming-stream:
  *     @ QString       http address zum customer interface
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETLOG_H
#define TCPCOMMAND_BACKUPMONITOR_GETLOG_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getLog : public TcpAbstractCommand
{
public:
    
    TcpCommand_backupMonitor_getLog();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_GETLOG_H



