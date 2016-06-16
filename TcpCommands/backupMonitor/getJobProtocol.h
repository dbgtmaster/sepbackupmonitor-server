/**
  * Liefert zu einem Job das Protokoll
  *
  * @incoming-stream:
  *     @ quint32 jobId:        ID des Jobs, von welchem das Protokoll geliefert werden soll
  *     @ quint32 protocolPage: Welche Seite vom Protokoll soll ermittelt werden?
  *     @ quint32 protocoltype: Welches Protokoll soll ermitellt werden, siehe PROTOCOL_TYPE
  *
  * @outcoming-stream:
  *     @ QByteArray:           Inhalt des Protokolls
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_GETJOBPROTOCOL_H
#define TCPCOMMAND_BACKUPMONITOR_GETJOBPROTOCOL_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_getJobProtocol : public TcpAbstractCommand
{
private:
    enum PROTOCOL_TYPE {
        MAIN,
        PRE,
        POST
    };

public:

    TcpCommand_backupMonitor_getJobProtocol();

    QVariant exec();

};

#endif // TCPCOMMAND_BACKUPMONITOR_GETJOBPROTOCOL_H


