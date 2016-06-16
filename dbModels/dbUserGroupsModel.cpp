#include "dbUserGroupsModel.h"

#include "TcpCommandsHelper.h"

#include <QDebug>
#include "MyLdap/MyLdap.h"

dbUserGroupsModel::dbUserGroupsModel() : dbAbstractModel() {

}

void dbUserGroupsModel::checkModelData() {

    if (_dbData["name"].toString().isEmpty()) {
        throw dbModelException("Es wurde kein Gruppenname angegeben.");
    }

    if (_dbData["name"].toString().count() > 45) {
        throw dbModelException("Gruppenname darf maximal 45 Zeichen lang sein.");
    }

    // Wenn LDAP Filter aktiviert, so dürfen die benötigten Eingabefelder nicht leer sein...
    if (_dbData["ldapfilter1_enabled"].toBool()) {
        if (_dbData["ldapfilter1_dn"].toString().size() <= 0) {
            throw dbModelException("LDAP Filter1: Es wurde keine DN angegeben.");
        }
        if (_dbData["ldapfilter1_filter"].toString().size() <= 0) {
            throw dbModelException("LDAP Filter1: Es wurde kein Filter angegeben.");
        }
    }
    if (_dbData["ldapfilter2_enabled"].toBool()) {
        if (_dbData["ldapfilter2_dn"].toString().size() <= 0) {
            throw dbModelException("LDAP Filter2: Es wurde keine DN angegeben.");
        }
        if (_dbData["ldapfilter2_filter"].toString().size() <= 0) {
            throw dbModelException("LDAP Filter2: Es wurde kein Filter angegeben.");
        }
    }
    if (_dbData["ldapfilter3_enabled"].toBool()) {
        if (_dbData["ldapfilter3_dn"].toString().size() <= 0) {
            throw dbModelException("LDAP Filter3: Es wurde keine DN angegeben.");
        }
        if (_dbData["ldapfilter3_filter"].toString().size() <= 0) {
            throw dbModelException("LDAP Filter3: Es wurde kein Filter angegeben.");
        }
    }
}

void dbUserGroupsModel::setFilter1(const TcpCommandsHelper::ldapFilter &filter) {
    _dbData["ldapfilter1_dn"] = filter.baseDn;
    _dbData["ldapfilter1_filter"] = filter.filter;

    switch (filter.scope) {
    case TcpCommandsHelper::LDAP_BASE:
        _dbData["ldapfilter1_scope"] = 0;
        break;
    case TcpCommandsHelper::LDAP_ONE:
        _dbData["ldapfilter1_scope"] = 1;
        break;
    case TcpCommandsHelper::LDAP_SUB:
        _dbData["ldapfilter1_scope"] = 2;
        break;
    default:
        _dbData["ldapfilter1_scope"] = 0;
    }
}

void dbUserGroupsModel::setFilter2(const TcpCommandsHelper::ldapFilter &filter) {
    _dbData["ldapfilter2_dn"] = filter.baseDn;
    _dbData["ldapfilter2_filter"] = filter.filter;

    switch (filter.scope) {
    case TcpCommandsHelper::LDAP_BASE:
        _dbData["ldapfilter2_scope"] = 0;
        break;
    case TcpCommandsHelper::LDAP_ONE:
        _dbData["ldapfilter2_scope"] = 1;
        break;
    case TcpCommandsHelper::LDAP_SUB:
        _dbData["ldapfilter2_scope"] = 2;
        break;
    default:
        _dbData["ldapfilter2_scope"] = 0;
    }
}

void dbUserGroupsModel::setFilter3(const TcpCommandsHelper::ldapFilter &filter) {
    _dbData["ldapfilter3_dn"] = filter.baseDn;
    _dbData["ldapfilter3_filter"] = filter.filter;

    switch (filter.scope) {
    case TcpCommandsHelper::LDAP_BASE:
        _dbData["ldapfilter3_scope"] = 0;
        break;
    case TcpCommandsHelper::LDAP_ONE:
        _dbData["ldapfilter3_scope"] = 1;
        break;
    case TcpCommandsHelper::LDAP_SUB:
        _dbData["ldapfilter3_scope"] = 2;
        break;
    default:
        _dbData["ldapfilter3_scope"] = 0;
    }
}

quint32 dbUserGroupsModel::create() {

    checkModelData();

    DatabaseQuery q = _db->insert(_dbData, "user_groups", "id");
    q.next();

    return q.row("id").toInt();
}

void dbUserGroupsModel::update(quint32 id) {

    checkModelData();

    _db->update(_dbData, "user_groups", "id", id);
}
