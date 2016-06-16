/**
  * Hallo Befehl zum TCP Server.. Sollte direkt nach dem Verbindungsaufbau ausgefuehrt werden...
  *
  * @incoming-stream:
  *     @ QString helo:   "hi" muss übergeben werden.
  *
  * @outcoming-stream:
  *     @ QString bool:     true, wenn eine Authentifizierung nötig ist, ansonsten false.
  */


#include <TcpCommands/main/connectTcp.h>

#include "Logger/Logger.h"
#include "Config.h"

#include "dbModels/dbUserModel.h"

TcpCommand_main_connectTcp::TcpCommand_main_connectTcp()
{
}

QVariant TcpCommand_main_connectTcp::exec() {

    QByteArray helo;
    _streamRead(helo);

    logNotice("Client says helo command '%1'", helo);

    if (helo != "hi") {
        logNotice("Client sends wrong helo command. Bye!");
        closeSocket();
        return true;
    }

    _streamWrite(QByteArray("hi"));

    return QVariant();
}

