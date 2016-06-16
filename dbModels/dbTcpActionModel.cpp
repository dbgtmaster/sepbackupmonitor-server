#include "dbTcpActionModel.h"

#include "Database/DatabasePool.h"
#include "Database/DatabaseQuery.h"

#include <QHash>

bool dbTcpAction::createIfNotExists(const QString &group, const QString &action) {

    DB db = DatabasePool::get();

    if (group.isEmpty()) throw dbModelException("groupname is empty!");
    if (action.isEmpty()) throw dbModelException("actionname is empty!");

    db->transaction();

    // Eintrag darf noch nicht existieren...
    DatabaseQuery q = db->getQuery();
    q.prepare("SELECT id FROM tcp_actions WHERE group_name = :group AND action_name = :action FOR UPDATE");
    q.bindValue(":group", group);
    q.bindValue(":action", action);
    q.exec();

    if (q.count() != 0) {
        db->rollback();
        return false;
    }

    // Ansonsten können wir den Eintrag anlegen...
    hashColumns cols;
    cols["group_name"] = group;
    cols["action_name"] = action;
    db->insert(cols, "tcp_actions");

    db->commit();

    return true;
}

DatabaseQuery dbTcpAction::updateSystemGroup(quint32 commandId, quint32 newSystemGroup) {

    DB db = DatabasePool::get();

    hashColumns cols;
    cols["system_group_id"] = newSystemGroup;

    return db->update(cols.begin(), cols.end(), "tcp_actions", "id", commandId);
}
