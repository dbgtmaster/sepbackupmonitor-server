#include "TcpCommands/system/saveTcpCommandsPermissions.h"

#include <dbModels/dbSystemGroupsModel.h>
#include <dbModels/dbTcpActionModel.h>

TcpCommand_system_saveTcpCommandsPermissions::TcpCommand_system_saveTcpCommandsPermissions() : TcpAbstractCommand(),
    _errorOccurred(false)
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_system_saveTcpCommandsPermissions::exec() {

    quint32 commandId;
    quint32 systemGroupId;
    while (!_streamReadAtAnd()) {

        _streamRead(commandId);
        _streamRead(systemGroupId);

        if (!dbSystemGroups::exists(systemGroupId)) {

            _errorOccurred = true;
            logError("Could not change permissions for commandID '%1', because systemgroup '%2' does not exists!",
                     QString::number(commandId), QString::number(systemGroupId));

            continue;
        }

        logNotice("Change tcpaction-id '%1' to systemgroup-id '%2'",
                  QString::number(commandId), QString::number(systemGroupId) );

        dbTcpAction::updateSystemGroup(commandId, systemGroupId);
    }

    _streamWrite( !_errorOccurred );

    return QVariant();
}

