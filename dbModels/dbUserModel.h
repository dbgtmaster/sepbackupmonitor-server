#ifndef DBUSERMODEL_H
#define DBUSERMODEL_H

#include "dbAbstractModel.h"

#include <QtGlobal>

class QString;

class dbUserModel : public dbAbstractModel
{
private:
    // Überprüft die Eingaben und schmeißt gegebenfalls ein dbModelException...
    void checkModelData();
public:
    dbUserModel();

    bool loadByLdapId(const QString &ldapId);

    void setUsername(const QString &username);
    void setLdapId(const QString &ldapId);
    void setSurName(const QString &sn);
    void setGivenName(const QString &givenName);
    void setDisplayName(const QString &displayName);
    void setMail(const QString &mail);

    // Benutzer erstellen:
    DatabaseQuery create();
};

namespace dbUser {

// Überprüfung, ob ein bestimmter Benutzer existiert...
bool checkExistsById(quint32 id);
bool checkExistsByLdapId(const QString& id, bool forUpdate = false);
bool checkExistsByUsername(const QString &username, bool forUpdate = false);

// Gibt zu einer UserId den Benutzernamen zurück...
QString getUsernameById(quint32 id);
}

#endif // DBUSERMODEL_H
