#ifndef UTILUSERS_H
#define UTILUSERS_H

#include <qglobal.h>

namespace UtilUsers {

// L�d die Mitgliedschaften von einem bestimmten Benutzer neu:
void reloadMemberships(quint32 userId);

// Gibt true zur�ck, wenn Benutzer eine bestimmte TCP- Action ausf�hren darf, ansonsten false.
// Wenn User noch nicht eingeloggt ist, so ist die userId "0" zu �bergeben!
bool canRunTcpCommand(quint32 userId, const QString &tcpGroup, const QString &tcpAction);
}

#endif // UTILUSERS_H
