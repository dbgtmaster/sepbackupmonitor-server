/**
  * Gibt eine Liste mit den User- Gruppen zurück...
  *
  * @incoming-stream:
  *     @ QStringList       Liste mit allen Spalten, welche ausgelesen werden sollen...
  *                         Wird nichts übergben, so wird automatisch "name" und "id" ausgewählt...
  *
  * @outcoming-stream:
  *     @ QMap<QString, QVariant>       Stellt einen Datensatz dar.
  *                         Wird so oft zurückgegeben, bis kein Datensatz mehr vorhanden ist...
  */

#ifndef TCPCOMMAND_ADMINISTRATION_GETALLUSERGROUPS_H
#define TCPCOMMAND_ADMINISTRATION_GETALLUSERGROUPS_H

#include <TcpAbstractCommand.h>

class TcpCommand_administration_getAllUserGroups : public TcpAbstractCommand
{
public:

    TcpCommand_administration_getAllUserGroups();

    QVariant exec();

};

#endif // TCPCOMMAND_ADMINISTRATION_GETALLUSERGROUPS_H


