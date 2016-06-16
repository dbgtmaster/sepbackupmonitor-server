/**
  * Sucht Kunden nach einem Filter und gibt eine Liste aller Ergebnisse zurück.
  *
  * @incoming-stream:
  *     @ QString       Suchstring
  *     @ QStringList   In welche Feldern soll durchsucht werden? (id, customer_name, address, plz, city)
  *     @ QStringList   Felder, welche zurückgegeben werden sollen..
  *                     id, customer_name, address, plz, ort
  *
  * @outcoming-stream:
  *     @ QList< QHash<QString, QVariant> >:
  *                     Jeder QStringList Eintrag stellt einen Kunden dar, im QHash (feld => wert) werden
  *                     die angeforderten Felder übermittelt.
  */

#ifndef TCPCOMMAND_CUSTOMERS_SEARCH_H
#define TCPCOMMAND_CUSTOMERS_SEARCH_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_search : public TcpAbstractCommand
{
public:

    TcpCommand_customers_search();

    QVariant exec();

};

#endif // TCPCOMMAND_CUSTOMERS_SEARCH_H


