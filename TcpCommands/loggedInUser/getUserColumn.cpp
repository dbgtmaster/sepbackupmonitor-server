/**
  * Teilt eine Information vom eingeloggten User mit.
  *
  * @incoming-stream:
  *     @ QString        Name der Spalte, von welcher die Information benötigt wird (Datenbankspalte!)
  *
  * @outcoming-stream:
  *     @ QString:          Inhalt der Spalte
  */

#include <TcpCommands/loggedInUser/getUserColumn.h>

#include <QByteArray>
#include "dbModels/dbUserModel.h"

TcpCommand_loggedInUser_getUserColumn::TcpCommand_loggedInUser_getUserColumn()
{
}

QVariant TcpCommand_loggedInUser_getUserColumn::exec() {

    QByteArray column;
    _streamRead(column);

    logDebug("TcpCommand_loggedInUser_getUserColumn::exec(): get Column: '%1'", column);
    QVariant write = _user->getColumn(column);
    _streamWrite(write);

    return QVariant();
}
