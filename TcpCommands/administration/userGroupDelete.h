/**
  * Löscht eine Benutzer- Gruppe
  *
  * @incoming-stream:
  *     @ quint32       ID der Benutzergruppe
  *
  * @outcoming-stream:
  *     @ bool          true wenn Benutzergruppe gelöscht wurde, ansonsten false...
  */

#ifndef TCPCOMMAND_ADMINISTRATION_USERGROUPDELETE_H
#define TCPCOMMAND_ADMINISTRATION_USERGROUPDELETE_H

#include <TcpAbstractCommand.h>

class TcpCommand_administration_userGroupDelete : public TcpAbstractCommand
{
public:

    TcpCommand_administration_userGroupDelete();

    QVariant exec();
    void postExec(const QVariant &returnStatus);
};

#endif // TCPCOMMAND_ADMINISTRATION_USERGROUPDELETE_H


