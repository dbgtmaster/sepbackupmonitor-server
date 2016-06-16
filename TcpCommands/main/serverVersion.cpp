/**
  * Teilt die aktuell laufende Serverversion mit.
  *
  * @incoming-stream:
  *     @ -
  *
  * @outcoming-stream:
  *     @ QByteArray:       String mit der Serverversion
  */

#include <TcpCommands/main/serverVersion.h>

#include "globals.h"

TcpCommand_main_serverVersion::TcpCommand_main_serverVersion()
{
}

QVariant TcpCommand_main_serverVersion::exec() {

    QByteArray version(VERSION);
    _streamWrite( version );

    return QVariant();
}
