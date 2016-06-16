#ifndef DBSYSTEMGROUPSMODEL_H
#define DBSYSTEMGROUPSMODEL_H

#include "Database/Database.h"

#include <QHash>

namespace dbSystemGroups {

    // Gibt eine Liste der Benutzer- Gruppen- IDs + deren Name zurück, welche Mitglied der
    // Systemgruppe sind...
    QHash<int, QString> getUserGroupsInSystemGroup(int id);

    // Überprüft, ob Systemgruppe existiert:
    bool exists(quint32 id);

    // Ermittelt die ID einer Systemgruppe anhand des Namens:
    // Wenn Name nicht existiert, wird -1 zurückgegeben.
    int getIdByName(const QString &name);
}

#endif // DBSYSTEMGROUPSMODEL_H

