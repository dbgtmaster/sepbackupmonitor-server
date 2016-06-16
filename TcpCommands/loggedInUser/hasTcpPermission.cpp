#include "TcpCommands/loggedInUser/hasTcpPermission.h"

#include "Utils/UtilUsers.h"
#include "dbModels/dbUserModel.h"

TcpCommand_loggedInUser_hasTcpPermission::TcpCommand_loggedInUser_hasTcpPermission() : TcpAbstractCommand()
{
}

/**
  * Fhrt den eigentlichen Tcp- Befehl aus...
  * Lesen: _streamRead(var)
  * Schreiben: _streamWrite(var);
  * Lesen & Schreiben werfen eine Exception, welche vom TcpThread aufgefangen wird, wenn ungltige Daten gelesen / geschrieben werden...
  * Auf Datastream direkt zugreifen (zB. um zu berprfen ob noch Daten vorhanden): _dataStreamRead, _dataStreamWrite
  * Socket close: _readSocket->close();
  *
  * @return QVariant: Es kann ein beliebiger Wert zurckgegeben werden... Wenn void postExec(const QVariant &var)  berladen wird,
  *						kann auf diesen Wert wieder zugegriffen werden.
  */
QVariant TcpCommand_loggedInUser_hasTcpPermission::exec() {
    
    QString group;
    QString action;
    _streamRead(group, action);

    _streamWrite( UtilUsers::canRunTcpCommand(_user->getColumn("id").toInt(), group, action) );
}


