#include "dbSystemGroupsModel.h"

#include "Database/DatabaseQuery.h"
#include "Database/DatabasePool.h"

#include "Logger/Logger.h"

#include <QVariant>

QHash<int, QString> dbSystemGroups::getUserGroupsInSystemGroup(int id) {

    DB db = DatabasePool::get();

    QHash<int, QString> list;
    DatabaseQuery q = db->query("SELECT gm.user_group AS id, ug.name FROM groups_memberships AS gm "
                                 " LEFT JOIN users_groups ug ON ug.id = gm.user_group "
                                 " WHERE system_group = " + id );
    while (q.next()) {
        list.insert( q.row("id").toInt(), q.row("name").toString() );
    }

    return list;
}

bool dbSystemGroups::exists(quint32 id) {

    DB db = DatabasePool::get();

    return db->checkExists(id, "id", "system_groups");
}

int dbSystemGroups::getIdByName(const QString &name) {

    DB db = DatabasePool::get();

    DatabaseQuery q = db->query("Select id FROM system_groups WHERE LOWER(name) = " + db->escape(name.toLower()));

    if (q.count() == 0) {
        logWarning("dbSystemGroups::getIdByName(): systemgroup name '%1' not found!", name);
        return -1;
    }

    q.next();
    int id = q.row("id").toInt();
    logDebug("dbSystemGroups::getIdByName(): Systemgroup id of group '%1' is '%2'", name, QString::number(id) );
    return id;
}
