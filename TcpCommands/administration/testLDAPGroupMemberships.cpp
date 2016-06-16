#include "TcpCommands/administration/testLDAPGroupMemberships.h"

#include "Logger/Logger.h"

#include <QSharedPointer>
#include <MyLdap/MyLdap.h>
#include <MyLdap/MyLdapResult.h>

TcpCommand_administration_testLDAPGroupMemberships::TcpCommand_administration_testLDAPGroupMemberships() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_administration_testLDAPGroupMemberships::exec() {

    QString username;
    _streamRead(username);

    TcpCommandsHelper::ldapFilter filter1;
    TcpCommandsHelper::ldapFilter filter2;
    TcpCommandsHelper::ldapFilter filter3;
    bool filter1Enabled;
    bool filter2Enabled;
    bool filter3Enabled;
    _streamRead(filter1);
    _streamRead(filter2);
    _streamRead(filter3);
    _streamRead(filter1Enabled);
    _streamRead(filter2Enabled);
    _streamRead(filter3Enabled);

    // Rückgabe:
    bool filter1Success;
    bool filter2Success;
    bool filter3Success;

    logNotice("Test group memberships by username '%1'", username);

    MyLdap ldap;
    ldap.connectByConfig();
    ldap.bindByConfig();

    if (!filter1Enabled) {
        logNotice("Filter 1 is disabled, continue with next filter...");
        filter1Success = false;
    }
    else {

        logNotice("Filter 1 settings:");
        logNotice("BaseDN: '%1'", filter1.baseDn);
        logNotice("Scope: '%1", QString::number(filter1.scope) );
        logNotice("Filter: '%1'", filter1.filter);

        // Im Filter %1 durch Benutzernamen ersetzen...
        filter1.filter = QString(filter1.filter).arg(username);

        QSharedPointer<MyLdapResult> res = ldap.search(filter1);

        if (res != 0) {
            int countEntries = res->countEntries();
            logNotice("Search returns %1 entries...", QString::number(countEntries));
            filter1Success = (countEntries != 0);
        }
        else {
            filter1Success = false;
        }
    }

    if (!filter2Enabled) {
        logNotice("Filter 2 is disabled, continue with next filter...");
        filter2Success = false;
    }
    else {

        logNotice("Filter 2 settings:");
        logNotice("BaseDN: '%1'", filter2.baseDn);
        logNotice("Scope: '%1", QString::number(filter2.scope) );
        logNotice("Filter: '%1'", filter2.filter);

        // Im Filter %1 durch Benutzernamen ersetzen...
        filter2.filter = QString(filter2.filter).arg(username);

        QSharedPointer<MyLdapResult> res = ldap.search(filter2);

        if (res != 0) {
            int countEntries = res->countEntries();
            logNotice("Search returns %1 entries...", QString::number(countEntries));
            filter2Success = (countEntries != 0);
        }
        else {
            filter2Success = false;
        }
    }

    if (!filter3Enabled) {
        logNotice("Filter 3 is disabled, continue with next filter...");
        filter3Success = false;
    }
    else {

        logNotice("Filter 3 settings:");
        logNotice("BaseDN: '%1'", filter3.baseDn);
        logNotice("Scope: '%1", QString::number(filter3.scope) );
        logNotice("Filter: '%1'", filter3.filter);

        // Im Filter %1 durch Benutzernamen ersetzen...
        filter3.filter = QString(filter3.filter).arg(username);

        QSharedPointer<MyLdapResult> res = ldap.search(filter3);

        if (res != 0) {
            int countEntries = res->countEntries();
            logNotice("Search returns %1 entries...", QString::number(countEntries));
            filter3Success = (countEntries != 0);
        }
        else {
            filter3Success = false;
        }
    }

    _streamWrite(filter1Success);
    _streamWrite(filter2Success);
    _streamWrite(filter3Success);

    return QVariant();
}

