/**
  * Überprüft, ob Benutzer in einer Bestimmten System- Gruppe ist...
  *
  * @incoming-stream:
  *     @ QString:      Name der SystemGruppe
  *
  * @outcoming-stream:
  *     @ bool:         true, wenn Benutzer in Gruppe ist, ansonsten false...
  */

#ifndef TCPCOMMAND_LOGGEDINUSER_ISINSYSTEMGROUP_H
#define TCPCOMMAND_LOGGEDINUSER_ISINSYSTEMGROUP_H

#include <TcpAbstractCommand.h>

class TcpCommand_loggedInUser_isInSystemGroup : public TcpAbstractCommand
{
public:

    TcpCommand_loggedInUser_isInSystemGroup();

    QVariant exec();

};

#endif // TCPCOMMAND_LOGGEDINUSER_ISINSYSTEMGROUP_H


