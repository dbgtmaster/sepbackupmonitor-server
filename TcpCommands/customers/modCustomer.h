/**
  * Bearbeitet einen Kunden
  *
  * @incoming-stream:
  *     @ quint32                       ID des Kunden
  *     @ QHash<QString, QVariant>      Spalten, welche beim Kunden bearbeitet werdne sollen.
  *
  * @outcoming-stream:
  *     @ QVariant                      bool true, wenn kein Fehler aufgetreten ist
  *                                     bool false, wenn kein Datensatz geändert wurde.
  *                                     QString msg, wenn ein Fehler aufgetreten ist (Fehlermeldungstext).
  *     @ quint32 customerid            ID des angelegten / bearbeitenden Kunden
  */

#ifndef TCPCOMMAND_CUSTOMERS_MODCUSTOMER_H
#define TCPCOMMAND_CUSTOMERS_MODCUSTOMER_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_modCustomer : public TcpAbstractCommand
{
private:

    quint32 _customerId;
public:

    TcpCommand_customers_modCustomer();

    QVariant exec();
    void postExec(const QVariant &returnStatus);

};

#endif // TCPCOMMAND_CUSTOMERS_MODCUSTOMER_H


