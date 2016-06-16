#include "UtilUsers.h"

#include "Database/DatabasePool.h"
#include "Database/DatabaseQuery.h"
#include <QVariant>
#include <QStringList>
#include <QReadWriteLock>

#include "dbModels/dbUserModel.h"

#include "MyLdap/MyLdap.h"

#include "Logger/Logger.h"
#include "Registry.h"

void UtilUsers::reloadMemberships(quint32 userId) {

    logNotice("Reload memberships auf userID '%1'", QString::number(userId));

    DB db = DatabasePool::get();

    QString username = dbUser::getUsernameById(userId);
    if (username.isEmpty()) {
        logWarning("UtilUsers::reloadMemberships(): Could not get username of userid %1. Exit reloading memberships.", QString::number(userId));
        return;
    }
    logNotice("UtilUsers::reloadMemberships(): Username of userId %1 is '%2'", QString::number(userId), username);

    MyLdap ldap;
    ldap.connectByConfig();
    ldap.bindByConfig();

    // Gruppen, in welche der Benutzer zugeordnet wird:
    QStringList groupsIds;

    DatabaseQuery q = db->query( QString("SELECT ldapfilter1_enabled, ldapfilter2_enabled, ldapfilter3_enabled, ")
                               +      "ldapfilter1_scope, ldapfilter2_scope, ldapfilter3_scope, "
                               +      "ldapfilter1_dn, ldapfilter2_dn, ldapfilter3_dn,"
                               +      "ldapfilter1_filter, ldapfilter2_filter, ldapfilter3_filter, "
                               +      "id, name"
                               + " FROM user_groups WHERE locked = false");

    while (q.next()) {

        logDebug("Run group %1, %2", q.row("id").toString(), q.row("name").toString());

        // Filter 1 aktiviert?
        if (q.row("ldapfilter1_enabled").toBool()) {

            int scope;
            switch (q.row("ldapfilter1_scope").toInt()) {
            case 0:
                scope = LDAP_SEARCH_BASE;
                break;
            case 1:
                scope = LDAP_SEARCH_ONE;
                break;
            case 2:
                scope = LDAP_SEARCH_SUB;
                break;
            }
            QSharedPointer<MyLdapResult> res = ldap.search(q.row("ldapfilter1_dn").toString(),
                                                           scope,
                                                            QString(q.row("ldapfilter1_filter").toString()).arg(username));

            if (res != 0) {
                int countEntries = res->countEntries();
                logNotice("Search returns %1 entries...", QString::number(countEntries));

                if (countEntries != 0) {
                    groupsIds.append(q.row("id").toString());
                    continue;
                }
            }
        }
        else {
            logNotice("Filter 1 is disabled, continue with next filter...");
        }

        // Filter 2 aktiviert?
        if (q.row("ldapfilter2_enabled").toBool()) {

            int scope;
            switch (q.row("ldapfilter2_scope").toInt()) {
            case 0:
                scope = LDAP_SEARCH_BASE;
                break;
            case 1:
                scope = LDAP_SEARCH_ONE;
                break;
            case 2:
                scope = LDAP_SEARCH_SUB;
                break;
            }
            QSharedPointer<MyLdapResult> res = ldap.search(q.row("ldapfilter2_dn").toString(),
                                                           scope,
                                                            QString(q.row("ldapfilter2_filter").toString()).arg(username));

            if (res != 0) {
                int countEntries = res->countEntries();
                logNotice("Search returns %1 entries...", QString::number(countEntries));

                if (countEntries != 0) {
                    groupsIds.append(q.row("id").toString());
                    continue;
                }
            }
        }
        else {
            logNotice("Filter 2 is disabled, continue with next filter...");
        }

        // Filter 3 aktiviert?
        if (q.row("ldapfilter3_enabled").toBool()) {

            int scope;
            switch (q.row("ldapfilter3_scope").toInt()) {
            case 0:
                scope = LDAP_SEARCH_BASE;
                break;
            case 1:
                scope = LDAP_SEARCH_ONE;
                break;
            case 2:
                scope = LDAP_SEARCH_SUB;
                break;
            }
            QSharedPointer<MyLdapResult> res = ldap.search(q.row("ldapfilter3_dn").toString(),
                                                           scope,
                                                            QString(q.row("ldapfilter3_filter").toString()).arg(username));

            if (res != 0) {
                int countEntries = res->countEntries();
                logNotice("Search returns %1 entries...", QString::number(countEntries));

                if (countEntries != 0) {
                    groupsIds.append(q.row("id").toString());
                    continue;
                }
            }
        }
        else {
            logNotice("Filter 3 is disabled, continue with next filter...");
        }
    }


    db->transaction();

    /**
      * Alle derzeitigen Zuordnungen löschen:
      */
    db->query("DELETE FROM user_memberships_to_groups WHERE user_id = " + QString::number(userId) );
    db->query("DELETE FROM user_memberships_to_systemgroups WHERE user_id = " + QString::number(userId) );

    if (groupsIds.size() > 0) {

        /**
          * Benutzer in deren Gruppen zuordnen:
          */
        QString sql = "INSERT INTO user_memberships_to_groups (user_id, group_id) VALUES ";
        for (QStringList::Iterator it = groupsIds.begin(); it != groupsIds.end(); it++) {

            if (it != groupsIds.begin()) sql = sql + ",";
            sql = sql + " ("+ QString::number(userId) +", "+ (*it) +")";
        }
        db->query(sql);

        /**
          * Benutzer in deren Benutzer- Gruppen zugehörigen Systemgruppen zuordnen:
          *
          * GROUP BY im SELECT Statement deshalb, damit die system_gruppen nicht doppelt eingefügt werden...
          */
        db->query(QString("INSERT INTO user_memberships_to_systemgroups (user_id, systemgroup_id) ")
                   + "(SELECT "+ QString::number(userId) +", system_group FROM groups_memberships WHERE user_group IN (" + groupsIds.join(",") + ") GROUP BY system_group)");
    }

    db->commit();
}

bool UtilUsers::canRunTcpCommand(quint32 userId, const QString &tcpGroup, const QString &tcpAction) {


    if (Registry::get("fullTcpPermissions").toBool()) {

        // Es wurde über Kommandozeile aktiviert, dass ein bestimmer Benutzer volle Berechtigungen hat...!
        // --fullTcpPermissions [username]

        if (Registry::get("fullTcpPermissionsUsername").toString().toLower() == dbUser::getUsernameById(userId).toLower()) {
            return true;
        }
    }

    DB db = DatabasePool::get();

    // Die ID der Systemgruppe "LoggedOff":
    static int systemGroupLoggedOffId = 0;
    static QReadWriteLock locker_systemGroupLoggedOffId;

    if (systemGroupLoggedOffId == 0) {
        QWriteLocker locker(&locker_systemGroupLoggedOffId);
        bool ok;
        logNotice("UtilUsers::canRunTcpCommand(): Get id of systemgroup 'loggedoff'");
        systemGroupLoggedOffId = db->query("SELECT id FROM system_groups WHERE name = 'LoggedOff'").fetchCol("id", &ok).toInt();
        if (!ok) {
            // Systemgruppe "LoggedOff" scheint nicht zu existieren, ein fataler Programm- Fehler!!!...
            logFatal("UtilUsers::canRunTcpCommand(): Fatal program error! Cannot find any system group named 'LoggedOff'!!!");
            systemGroupLoggedOffId = 0;
            return false;
        }
        locker.unlock();
    }

    logNotice("UtilUsers::canRunTcpCommand(): Check permissions by userid %1, action %2/%3", QString::number(userId), tcpGroup, tcpAction);

    // Die Systemgruppe des TCP Befehls auslesen:
    bool ok;
    int systemGroupId = db->query("SELECT system_group_id FROM tcp_actions WHERE group_name = "+ db->escape(tcpGroup) + " AND action_name = "+ db->escape(tcpAction)).fetchCol("system_group_id", &ok).toInt();
    if (!ok || systemGroupId == 0 || systemGroupId == -1) {
        return false;
    }

    // Wenn UserId 0 ist, so ist Benutzer nicht eingeloggt und darf nur Aktionen der Systemgruppe "LoggedOff" ausführen...
    if (userId == 0) {
        QReadLocker locker(&locker_systemGroupLoggedOffId);
        return (systemGroupLoggedOffId == systemGroupId);
    }

    // Überprüfen, ob sich Benutzer in dieser TCP Gruppe befindet:
    int isInGroup = db->query("SELECT COUNT(*) AS count FROM user_memberships_to_systemgroups WHERE user_id = " + QString::number(userId) + " AND systemgroup_id = " + QString::number(systemGroupId))
            .fetchCol("count").toInt();

    return (isInGroup > 0);
}
