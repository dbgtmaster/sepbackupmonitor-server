/**
  * Gibt die letzten Benachhrichtigungen zur√ºck
  *
  * @incoming-stream:
  *     @ datatype		Beschreibung
  *
  * @outcoming-stream:
  *     @ QList<QHash<QString, QVariant>>		QList beinhaltet die einzelnen Benachrichtigungen, QHash die Werte, dies sind:
  *                                                 - source: Wer diese Meldung produziert hat, zB. backup
  *                                                 - data: ein qt serialisiertes array
  *                                                 - prioriy: 0 -> normale nachricht, 1 -> nachricht mit priorit‰t (hervorhebung), 2 -> Nachricht poppt bei allen Benutzern auf
  */

#ifndef TCPCOMMAND_NOTIFICATIONS_GETLASTMESSAGES_H
#define TCPCOMMAND_NOTIFICATIONS_GETLASTMESSAGES_H

#include <TcpAbstractCommand.h>

class TcpCommand_notifications_getLastMessages : public TcpAbstractCommand
{
public:
    
    TcpCommand_notifications_getLastMessages();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_NOTIFICATIONS_GETLASTMESSAGES_H



