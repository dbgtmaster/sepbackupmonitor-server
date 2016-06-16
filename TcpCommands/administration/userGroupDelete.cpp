#include "TcpCommands/administration/userGroupDelete.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"

TcpCommand_administration_userGroupDelete::TcpCommand_administration_userGroupDelete() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_administration_userGroupDelete::exec() {

    quint32 id;
    _streamRead(id);

    logNotice("Delete usergroup with id '%1'", QString::number(id));

    _db->remove("user_groups", "id", id);

    return true;
}

void TcpCommand_administration_userGroupDelete::postExec(const QVariant &returnStatus) {

    _streamWrite(returnStatus.toBool());
}
