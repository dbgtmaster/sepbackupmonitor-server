/**
  * Gibt eine Liste mit allen verfügbaren Tcp- Commands zurück...
  *
  * @incoming-stream:
  *     @ nothing
  *
  * @outcoming-stream:
  *     @ quint32       ID des Tcp- Befehls
  *     @ QByteArray    Gruppenname des TCP- Befehls
  *     @ QByteArray    Name des Befehls
  *     @ quint32       System- Gruppen- ID, in welcher sich dieser Befehl befindet.
  *                     -1 bedeutet, Befehl befindet sich in noch keiner Gruppe...
  *         -> Diese 4 Rückgaben werden in der Schleife zurückgegeben, bis kein Befehl mehr vorhanden ist...
  */

#ifndef TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDS_H
#define TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDS_H

#include <TcpAbstractCommand.h>

class TcpCommand_system_getAllTcpCommands : public TcpAbstractCommand
{
public:

    TcpCommand_system_getAllTcpCommands();

    QVariant exec();

};

#endif // TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDS_H


