/**
  * Liefert Details zu einer bestimmten UserGruppen
  *
  * @incoming-stream:
  *     @ quint32:      ID der UserGruppe
  *     @ QStringList:  Spalten, welche benötigt werden.
  *     @ bool:         Soll auch eine Liste zurückgegeben werden, in welche Systemgruppen
  *                     sich diese Gruppe befindet?
  *
  * @outcoming-stream:
  *     @ mapColumns:   Gibt eine Liste der Spalten zurück...
  *                     Ist die Liste leer, so wurde kein Datensatz gefunden...
  *     @ QList<quint32>:   Liste, in welchen SystemGruppen- IDs sich diese Gruppe befindet.
  *                     Wurde eine Liste nicht angefordert, so wird eine leere Liste zurückgegeben...
  */

#ifndef TCPCOMMAND_ADMINISTRATION_GETUSERGROUP_H
#define TCPCOMMAND_ADMINISTRATION_GETUSERGROUP_H

#include <TcpAbstractCommand.h>

class TcpCommand_administration_getUserGroup : public TcpAbstractCommand
{
public:

    TcpCommand_administration_getUserGroup();

    QVariant exec();

};

#endif // TCPCOMMAND_ADMINISTRATION_GETUSERGROUP_H


