/**
  * Ändert / speichert Berechtigungen für Tcp- Befehle
  *
  * @incoming-stream:
  *     @ quint32 :         ID des Tcp- Befehls
  *     @ quint32 :         ID der neuen Systemgruppe, welche gesetzt werden soll...
  *
  *             -> Diese 2 Befehle werden wiederholend übertragen!
  *
  * @outcoming-stream:
  *     @ bool      true, wenn Änderungen durchgeführt wurden, ansosten false, wenn Fehler aufgetreten sind...
  */

#ifndef TCPCOMMAND_SYSTEM_SAVETCPCOMMANDSPERMISSIONS_H
#define TCPCOMMAND_SYSTEM_SAVETCPCOMMANDSPERMISSIONS_H

#include <TcpAbstractCommand.h>

class TcpCommand_system_saveTcpCommandsPermissions : public TcpAbstractCommand
{
private:

    bool _errorOccurred;
public:

    TcpCommand_system_saveTcpCommandsPermissions();

    QVariant exec();

};

#endif // TCPCOMMAND_SYSTEM_SAVETCPCOMMANDSPERMISSIONS_H


