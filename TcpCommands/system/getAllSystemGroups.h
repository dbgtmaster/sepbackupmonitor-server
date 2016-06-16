/**
  * Gibt eine Liste mit allen existieredenn Systemgruppen zurück.
  *
  * @incoming-stream:
  *     @ bool              true, wenn auch Beschreibung der SysGroup zum Client übertragen werden soll.
  *                         bei false wird diese nicht übertragen...
  *
  * @outcoming-stream:
  *     @ quint32           ID der Systemgruppe
  *     @ QString           Name der Systemgruppe
  *     @ QString           Beschreibung der SystemGruppe
  *       [CONDITION]       Nur, wenn auch die Beschreibung angefordert wurde!
  *
  *     Die Parameter werden so lange in einer Schleife zurückgegeben, bis keine weiteren Daten mehr vorhanden sind.
  */

#ifndef TCPCOMMAND_SYSTEM_GETALLSYSTEMGROUPS_H
#define TCPCOMMAND_SYSTEM_GETALLSYSTEMGROUPS_H

#include <TcpAbstractCommand.h>

class TcpCommand_system_getAllSystemGroups : public TcpAbstractCommand
{
public:

    TcpCommand_system_getAllSystemGroups();

    QVariant exec();

};

#endif // TCPCOMMAND_SYSTEM_GETALLSYSTEMGROUPS_H


