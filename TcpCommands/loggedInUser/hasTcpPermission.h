/**
  * Überprüft, ob Benutzer bestimmte Berechtigung hat...
  *
  * @incoming-stream:
  *     @ QString groupName         Name der Gruppe, in welcher sich diese Aktion befindet.
  *     @ QString actionName        Name der Aktion
  *
  * @outcoming-stream:
  *     @ bool                      bool true, wenn Benutzer diese Berechtigung hat, ansonsten false.
  */

#ifndef TCPCOMMAND_LOGGEDINUSER_HASTCPPERMISSION_H
#define TCPCOMMAND_LOGGEDINUSER_HASTCPPERMISSION_H

#include <TcpAbstractCommand.h>

class TcpCommand_loggedInUser_hasTcpPermission : public TcpAbstractCommand
{
public:
    
    TcpCommand_loggedInUser_hasTcpPermission();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_LOGGEDINUSER_HASTCPPERMISSION_H



