/**
  * Gibt die http Adresse zum customer interface zurück...
  *
  * @incoming-stream:
  *     @ none
  *
  * @outcoming-stream:
  *     @ QString       http address zum customer interface
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H
#define TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getCustomerInterfaceAddress : public TcpAbstractCommand
{
public:
    
    TcpCommand_backupMonitor_getCustomerInterfaceAddress();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_GETCUSTOMERINTERFACEADDRESS_H



