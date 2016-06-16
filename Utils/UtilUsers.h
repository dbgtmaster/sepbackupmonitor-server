#ifndef UTILUSERS_H
#define UTILUSERS_H

#include <qglobal.h>

namespace UtilUsers {

// Läd die Mitgliedschaften von einem bestimmten Benutzer neu:
void reloadMemberships(quint32 userId);

// Gibt true zurück, wenn Benutzer eine bestimmte TCP- Action ausführen darf, ansonsten false.
// Wenn User noch nicht eingeloggt ist, so ist die userId "0" zu übergeben!
bool canRunTcpCommand(quint32 userId, const QString &tcpGroup, const QString &tcpAction);
}

#endif // UTILUSERS_H
