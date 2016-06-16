#ifndef DBUSERGROUPSMODEL_H
#define DBUSERGROUPSMODEL_H

#include "dbAbstractModel.h"

namespace TcpCommandsHelper {
    struct ldapFilter;
}

class dbUserGroupsModel : public dbAbstractModel
{
private:

    enum Action {
        CREATE, MODIFY
    };

    void checkModelData();
public:
    dbUserGroupsModel();

    inline void setId(const quint32 id);
    inline void setName(const QString &name);
    inline void setIsLocked(bool t);
    inline void setFilter1Enabled(bool t);
    inline void setFilter2Enabled(bool t);
    inline void setFilter3Enabled(bool t);
    void setFilter1(const TcpCommandsHelper::ldapFilter &filter);
    void setFilter2(const TcpCommandsHelper::ldapFilter &filter);
    void setFilter3(const TcpCommandsHelper::ldapFilter &filter);

    // Gibt die ID des erstellten Datensatzes zurück...
    quint32 create();

    void update(quint32 id);
};

void dbUserGroupsModel::setName(const QString &name) {
    _dbData["name"] = name;
}

void dbUserGroupsModel::setIsLocked(const bool t) {
    _dbData["locked"] = t;
}

void dbUserGroupsModel::setFilter1Enabled(bool t) {
    _dbData["ldapfilter1_enabled"] = t;
}

void dbUserGroupsModel::setFilter2Enabled(bool t) {
    _dbData["ldapfilter2_enabled"] = t;
}

void dbUserGroupsModel::setFilter3Enabled(bool t) {
    _dbData["ldapfilter3_enabled"] = t;
}

#endif // DBUSERGROUPSMODEL_H

