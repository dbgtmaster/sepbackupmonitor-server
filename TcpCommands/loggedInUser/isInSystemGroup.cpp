#include "TcpCommands/loggedInUser/isInSystemGroup.h"

TcpCommand_loggedInUser_isInSystemGroup::TcpCommand_loggedInUser_isInSystemGroup() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_loggedInUser_isInSystemGroup::exec() {

    return QVariant();
}

