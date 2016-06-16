/**
  * Verwaltet die Mitgliedschaft von einer Gruppe zu Systemgruppen
  *
  * @incoming-stream:
  *     @ quint32           ID der Benutzergruppe
  *     @ QList<quint32>    Liste mit IDs, in welchen Systemgruppen sich diese Gruppe befindet.
  *
  * @outcoming-stream:
  *     @ nothing
  */

#ifndef TCPCOMMAND_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H
#define TCPCOMMAND_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H

#include <TcpAbstractCommand.h>

class TcpCommand_administration_userGroupToSystemGroups : public TcpAbstractCommand
{
public:

    TcpCommand_administration_userGroupToSystemGroups();

    QVariant exec();

};

#endif // TCPCOMMAND_ADMINISTRATION_USERGROUPTOSYSTEMGROUPS_H


