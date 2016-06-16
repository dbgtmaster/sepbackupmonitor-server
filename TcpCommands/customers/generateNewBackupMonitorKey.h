/**
  * Generiert einen neuen Key für das Backup- Monitor Interface und schreibt diesen in die Datenbank.
  *
  * @incoming-stream:
  *     @ quint32       ID des Kunden
  *
  * @outcoming-stream:
  *     @ QString       Der neue Interface Key
  */

#ifndef TCPCOMMAND_CUSTOMERS_GENERATENEWBACKUPMONITORKEY_H
#define TCPCOMMAND_CUSTOMERS_GENERATENEWBACKUPMONITORKEY_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_generateNewBackupMonitorKey : public TcpAbstractCommand
{
public:

    TcpCommand_customers_generateNewBackupMonitorKey();

    QVariant exec();

};

#endif // TCPCOMMAND_CUSTOMERS_GENERATENEWBACKUPMONITORKEY_H


