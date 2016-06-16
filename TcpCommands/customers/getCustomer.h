/**
  * Liefert die Daten eines Kunden...
  *
  * @incoming-stream:
  *     @ quint32       ID des Kunden
  *     @ QStringList   Spalten, welche vom Kunden ausgelesen werden sollen??
  *
  * @outcoming-stream:
  *     @ QHash<QString, QVariant>      Die angeordeten Daten des Kunden, geordnet nach Spalte -> Wert
  */

#ifndef TCPCOMMAND_CUSTOMERS_GETCUSTOMER_H
#define TCPCOMMAND_CUSTOMERS_GETCUSTOMER_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_getCustomer : public TcpAbstractCommand
{
public:

    TcpCommand_customers_getCustomer();

    QVariant exec();

};

#endif // TCPCOMMAND_CUSTOMERS_GETCUSTOMER_H


