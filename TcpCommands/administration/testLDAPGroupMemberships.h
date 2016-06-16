/**
  * Überprüft, ob anhand eines Benutzernamens die LDAP Filter eine Gruppe zutreffen würden und somit,
  *  ob Benutzer Mitglied dieser Gruppe sein würde.
  *
  * @incoming-stream:
  *     @ QString:      Benutzename, mit welchem geprüft werden soll.
  *     @ TcpCommandsHelper::ldapFilter:
  *                     Der erste LDAP Filter, mit welchem die Mitgliedschaft überprüft werden soll.
  *     @ TcpCommandsHelper::ldapFilter:
  *                     Der zweite LDAP Filter, mit welchem die Mitgliedschaft überprüft werden soll.
  *     @ TcpCommandsHelper::ldapFilter:
  *                     Der dritte LDAP Filter, mit welchem die Mitgliedschaft überprüft werden soll.
  *     @ bool:         Ob Filter 1 aktiviert ist (true -> Filter ist aktiviert).
  *                     ACHTUNG: Wird false übergeben, so wird der Filter automatisch als fehlgeschlagen übergeben.
  *     @ bool:         Ob Filter 2 aktiviert ist (true -> Filter ist aktiviert).
  *                     ACHTUNG: Wird false übergeben, so wird der Filter automatisch als fehlgeschlagen übergeben.
  *     @ bool:         Ob Filter 3 aktiviert ist (true -> Filter ist aktiviert).
  *                     ACHTUNG: Wird false übergeben, so wird der Filter automatisch als fehlgeschlagen übergeben.
  *
  * @outcoming-stream:
  *     @ bool          Ob Filter 1 erfolgreich war (true -> war erfolgreich)
  *     @ bool          Ob Filter 2 erfolgreich war (true -> war erfolgreich)
  *     @ bool          Ob Filter 3 erfolgreich war (true -> war erfolgreich)
  */

#ifndef TCPCOMMAND_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H
#define TCPCOMMAND_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H

#include <TcpAbstractCommand.h>

class TcpCommand_administration_testLDAPGroupMemberships : public TcpAbstractCommand
{
public:

    TcpCommand_administration_testLDAPGroupMemberships();

    QVariant exec();

};

#endif // TCPCOMMAND_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPS_H


