#include "dbUserModel.h"

#include <QVariant>
#include <QSqlQuery>

#include "Database/DatabasePool.h"

dbUserModel::dbUserModel() : dbAbstractModel() {

}

bool dbUserModel::loadByLdapId(const QString &ldapId) {

    DB db = DatabasePool::get();

    _dbData = db->select("*", "ldap_id", ldapId, "users");

    // Wenn mehr als 1 Spalte geladen wurde, so ist das Objekt gültig...
    return setValid( _dbData.size() > 0 );
}

void dbUserModel::setUsername(const QString &val) {
    _dbData["username"] = val;
}

void dbUserModel::setLdapId(const QString &val) {
    _dbData["ldap_id"] = val;
}

void dbUserModel::setSurName(const QString &val) {
    _dbData["surname"] = val;
}

void dbUserModel::setGivenName(const QString &val) {
    _dbData["givenname"] = val;
}

void dbUserModel::setMail(const QString &val) {
    _dbData["mail"] = val;
}

void dbUserModel::setDisplayName(const QString &val) {
    _dbData["displayname"] = val;
}

DatabaseQuery dbUserModel::create() {

    checkModelData();
    return DatabasePool::get()->insert(_dbData, "users");
}

void dbUserModel::checkModelData() {

    QString username = _dbData.value("username").toString();
    if (username.size() < 2) {
        throw dbModelException("Benutzername muss mindestens 2 Zeichen lang sein.");
    }
    if (username.size() > 150) {
        throw dbModelException("Benutzername darf maximal 150 Zeichen lang sein.");
    }
    /**
      * @TODO
      * .. weitere Checks
      */
}

bool dbUser::checkExistsById(quint32 id) {
    return DatabasePool::get()->checkExists(QVariant(id), "id", "users");
}

bool dbUser::checkExistsByLdapId(const QString& id, bool forUpdate) {
    return DatabasePool::get()->checkExists(QVariant(id), "ldap_id", "users", forUpdate);
}

bool dbUser::checkExistsByUsername( const QString &username, bool forUpdate) {
    return DatabasePool::get()->checkExists(QVariant(username), "username", "users", forUpdate);
}

QString dbUser::getUsernameById(quint32 id) {

    DB db = DatabasePool::get();

    DatabaseQuery q = db->query("SELECT username FROM users WHERE id = " + QString::number(id));

    q.next();
    return q.row("username").toString();
}
