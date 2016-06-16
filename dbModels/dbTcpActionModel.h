#ifndef DBTCPACTIONMODEL_H
#define DBTCPACTIONMODEL_H

#include "dbModels/dbAbstractModel.h"

class Database;

namespace dbTcpAction {

// Erstellt iene Tcp- Aktion in der Datenbank, falls diese noch nicht existiert...
// bool true, wenn Aktion noch nicht existiert hat und angelegt wurde, ansonsten false...
bool createIfNotExists(const QString &group, const QString &action);

// Ändert die aktuelle Systemgruppe...
DatabaseQuery updateSystemGroup(quint32 commandId, quint32 newSystemGroup);

}

#endif // DBTCPACTIONMODEL_H
