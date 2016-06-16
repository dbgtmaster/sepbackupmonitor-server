#include "TcpCommands/administration/userGroupToSystemGroups.h"

#include "Logger/Logger.h"
#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

TcpCommand_administration_userGroupToSystemGroups::TcpCommand_administration_userGroupToSystemGroups() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_administration_userGroupToSystemGroups::exec() {

    quint32 groupId;
    QList<quint32> memberships;

    _streamRead(groupId);
    _streamRead(memberships);

    _db->transaction();

    _db->query("DELETE FROM groups_memberships WHERE user_group = " + QString::number(groupId) );

    foreach(quint32 sysGroupId, memberships) {

        logNotice("Add systemgroup '%1' to group '%2'", QString::number(sysGroupId), QString::number(groupId) );

        _db->query("INSERT INTO groups_memberships (user_group, system_group) VALUES ("+ QString::number(groupId) +", "+ QString::number(sysGroupId) +")");
    }

    _db->commit();

    return QVariant();
}

