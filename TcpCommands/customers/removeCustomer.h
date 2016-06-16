/**
  * Löscht einen Kunden...
  * Nur möglich, wenn in der Konfig [customers,editable=yes] gesetzt ist.
  *
  * @incoming-stream:
  *     @ quint32		Kundenid
  *
  * @outcoming-stream:
  *     @ none
  */

#ifndef TCPCOMMAND_CUSTOMERS_REMOVECUSTOMER_H
#define TCPCOMMAND_CUSTOMERS_REMOVECUSTOMER_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_removeCustomer : public TcpAbstractCommand
{
public:
    
    TcpCommand_customers_removeCustomer();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_CUSTOMERS_REMOVECUSTOMER_H



