#include "TcpCommands/administration/userGroupMod.h"

#include "dbModels/dbUserGroupsModel.h"

#include "Logger/Logger.h"

TcpCommand_administration_userGroupMod::TcpCommand_administration_userGroupMod() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_administration_userGroupMod::exec() {

    TcpCommandsHelper::Action action;
    quint32 action_, id;
    QString name;
    bool isLocked;
    bool filter1Enabled;
    bool filter2Enabled;
    bool filter3Enabled;
    TcpCommandsHelper::ldapFilter filter1;
    TcpCommandsHelper::ldapFilter filter2;
    TcpCommandsHelper::ldapFilter filter3;

    _streamRead(action_);
    _streamRead(id);
    _streamRead(name);
    _streamRead(isLocked);
    _streamRead(filter1Enabled);
    _streamRead(filter2Enabled);
    _streamRead(filter3Enabled);
    _streamRead(filter1);
    _streamRead(filter2);
    _streamRead(filter3);

    action = static_cast<TcpCommandsHelper::Action>(action_);

    if (action == TcpCommandsHelper::CREATE) {
        logNotice("Try to create usergroup %1", name);
    }
    else {
        logNotice("Try to modify usergroup %1", name);
    }

    dbUserGroupsModel group;
    group.setName(name);
    group.setIsLocked(isLocked);
    group.setFilter1(filter1);
    group.setFilter2(filter2);
    group.setFilter3(filter3);
    group.setFilter1Enabled(filter1Enabled);
    group.setFilter2Enabled(filter2Enabled);
    group.setFilter3Enabled(filter3Enabled);

    try {
        if (action == TcpCommandsHelper::CREATE) {
            id = group.create();
        }
        else {
            group.update(id);
        }
    }
    catch (dbModelException &e) {

        // Ein Fehler ist aufgetreten...
        _streamWrite(false);
        _streamWrite(id);
        _streamWrite(e.getMessage());
        logNotice("Returned error message: '%1'", e.getMessage());
        return false;
    }

    // Alles gut gelaufen...
    _streamWrite(true);
    _streamWrite(id);
    _streamWrite(QString());

    return QVariant();
}
