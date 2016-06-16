/**
  * Erstellt bzw. bearbeitet eine Benutzergruppe
  *
  * @incoming-stream:
  *     @ ::Action:         Ob Gruppe erstellt bzw bearbeitet wird.
  *     @ quint32           ID der Gruppe (nur wenn bearbeitet wird, bei erstellen wird Parameter ignoriert)
  *     @ QString           Name der Benutzergruppe
  *     @ bool              Deaktiviert? true -> Gruppe ist deaktiviert.
  *     @ bool              Ist LDAPFilter1 aktiviert?
  *     @ bool              Ist LDAPFilter2 aktiviert?
  *     @ bool              Ist LDAPFilter3 aktiviert?
  *     @ TcpCommandsHelper::ldapFilter
  *                         Erster LDAP- Filter
  *     @ TcpCommandsHelper::ldapFilter
  *                         Zweiter LDAP- Filter
  *     @ TcpCommandsHelper::ldapFilter
  *                         Dritter LDAP- Filter
  *
  * @outcoming-stream:
  *     @ bool              Gruppe erfolgreich erstellt bzw bearbeitet?
  *     @ id                Id der erstellten bzw. bearbeitenden Gruppe
  *     @ QString           Fehlermeldungstext, warum Gruppe nicht erstellt/ bearbeitet werden konnte.
  */

#ifndef TCPCOMMAND_ADMINISTRATION_USERGROUPMOD_H
#define TCPCOMMAND_ADMINISTRATION_USERGROUPMOD_H

#include <TcpAbstractCommand.h>

class TcpCommand_administration_userGroupMod : public TcpAbstractCommand
{

public:

    TcpCommand_administration_userGroupMod();

    QVariant exec();

};

#endif // TCPCOMMAND_ADMINISTRATION_USERGROUPMOD_H


