/**
  * Liefert Laufzeitinformationen zum Intranet Server
  *
  * @incoming-stream:
  *     @ none
  *
  * @outcoming-stream:
  *     @ qHash<QString key, QVariant value>		Laufzeitinformationen
  */

#ifndef TCPCOMMAND_SYSTEM_SERVERSTATISTICS_H
#define TCPCOMMAND_SYSTEM_SERVERSTATISTICS_H

#include <TcpAbstractCommand.h>

class TcpCommand_system_serverStatistics : public TcpAbstractCommand
{
public:
    
    TcpCommand_system_serverStatistics();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_SYSTEM_SERVERSTATISTICS_H



