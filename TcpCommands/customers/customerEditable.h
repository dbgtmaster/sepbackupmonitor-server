/**
  * Mit dieser Funktion kann überprüft werden, ob das bearbeiten bzw. erstellen der Kundendaten (Kundenname, Adresse,...) aktiviert wurde.
  *
  * @incoming-stream:
  *     @ none		Beschreibung
  *
  * @outcoming-stream:
  *     @ bool		true, wenn aktiviert, ansonsten false.
  */

#ifndef TCPCOMMAND_CUSTOMERS_CUSTOMEREDITABLE_H
#define TCPCOMMAND_CUSTOMERS_CUSTOMEREDITABLE_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_customerEditable : public TcpAbstractCommand
{
public:
    
    TcpCommand_customers_customerEditable();
    
    QVariant exec();
    
    //void postExec(const QVariant &var);	
    
};

#endif // TCPCOMMAND_CUSTOMERS_CUSTOMEREDITABLE_H



