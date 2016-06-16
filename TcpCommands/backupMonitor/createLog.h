/**
  * Gibt das Log von einem Kunden bzw. Job aus.
  *
  * @incoming-stream:
  *     @ none
  *
  * @outcoming-stream:
  *     @ QString       http address zum customer interface
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_CREATELOG_H
#define TCPCOMMAND_BACKUPMONITOR_CREATELOG_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_createLog : public TcpAbstractCommand
{
public:
    
    TcpCommand_backupMonitor_createLog();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_CREATELOG_H



