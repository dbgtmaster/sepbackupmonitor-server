/**
  * Erstellt eine neue Benachrichtigung
  *
  * @incoming-stream:
  *     @ QByteArray type           Art der Benachrichtigung, zB. "backup"
  *     @ QByteArray data           Daten, geschrieben von QDataStream, welche zur Darstellung der Benachrichtigung benötigt werden.
  *     @ quint8 priority           0 -> Normale Benachrichtigung, 1 -> Benachrichtigung wird hervorgehoben, 2 -> Benachrichtigung poppt auf, wenn User eingeloggt ist.
  *
  * @outcoming-stream:
  *     @ none
  */

#ifndef TCPCOMMAND_NOTIFICATIONS_CREATE_H
#define TCPCOMMAND_NOTIFICATIONS_CREATE_H

#include <TcpAbstractCommand.h>

class TcpCommand_notifications_create : public TcpAbstractCommand
{
public:
    
    TcpCommand_notifications_create();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_NOTIFICATIONS_CREATE_H



