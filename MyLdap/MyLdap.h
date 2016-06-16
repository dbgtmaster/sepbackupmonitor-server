/*
 * File:   MyLdap.h
 * Author: Thomas Manninger, Siedl Networks GmbH, www.siedl.net
 *
 * Copyright (C) 2010 Siedl Networks GmbH
 *
 * http://www.siedl.net/
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Created on 20. Juli 2010, 09:25
 */

#ifndef _MYLDAP_H
#define	_MYLDAP_H

#include <iostream>
#include <string>
#include <QSharedPointer>

#define LDAP_DEPRECATED 1
#include <ldap.h>
#include "../Logger/Logger.h"
#include "MyLdap/MyLdapResult.h"

#define LDAP_SEARCH_BASE LDAP_SCOPE_BASE
#define LDAP_SEARCH_ONE LDAP_SCOPE_ONE
#define LDAP_SEARCH_SUB LDAP_SCOPE_SUB

class MyLdapResult;
class QString;

namespace TcpCommandsHelper {
    struct ldapFilter;
}

class MyLdap {
private:
    LDAP *_ld;

    // Falls während der Sitzung ein Fehler auftritt, damit ein erneuter
    // Bind gestartet werden kann...
    QString _hostname;
    QString _bindDn;
    QString _password;

    static MyLdap* _instance;

    QList<MyLdapResult*> _openedLdapResults;

public:

    // Verbindung aufgebaut?
    bool _isConnected;

    // Entweder man erstellt das Objekt über instance(), um ein globales Objekt zu haben,
    // oder man erstellt eines direkt über den Konstruktor, je nach Geschmack...
    MyLdap();

    ~MyLdap();

    static MyLdap* instance();
    static void deleteInstance();

    // We use ldap_init(), and this function does not open the connection!
    // In opens the connection by the first ldap operation, so this method connect() never
    // returns an error!
    void connect(const QString& hostname);
    void connectByConfig();

    // Wenn bindDn leer ist, so wird anonym gebunden...
    bool bind(const QString& bindDn, const QString& password);
    bool bindByConfig();
    void unbind();

    // ACHTUNG:
    // Bitte nach der Suche überprüfen, ob nicht ein "0"- Pointer Objekt
    // zurückgegeben wurde... Wenn nicht überprüft wird, kann sonst das Programm
    // abstürzen!!
    QSharedPointer<MyLdapResult> search(const QString& dn, int scope, const QString& filter);

    // Sucht anhand eines LDAP- Filters im LDAP.
    // Liefert die Anzahl der gefundenen Ergebnisse zurück.
    // ACHTUNG:
    // Bitte nach der Suche überprüfen, ob nicht ein "0"- Pointer Objekt
    // zurückgegeben wurde... Wenn nicht überprüft wird, kann sonst das Programm
    // abstürzen!!
    QSharedPointer<MyLdapResult> search(const TcpCommandsHelper::ldapFilter &filter);

    LDAPMessage* firstEntry(LDAPMessage *res);
    LDAPMessage* nextEntry(LDAPMessage *res);

    char* firstAttribute(LDAPMessage *res, BerElement **ptr);
    char* nextAttribute(LDAPMessage *res, BerElement *ptr);
    char** getAttrValues(LDAPMessage *res, char *attribute);
    int countValues(char** values);
    QString getEntryDn(LDAPMessage *res);

    void memFree(char *dn);
    void berFree(BerElement *ber);
    void valueFree(char **vals);
    void msgFree(LDAPMessage *msg);
    void modsFree(LDAPMod **mod);


    QString err2String(int errCode);

    QString escapeSearchFilter(const QString& toEscape);
    QString escapeDn(const QString& toEscape);

    void freeRessources();

    int countEntries(LDAPMessage *res);
};

class LdapException {
private:
    QString _message;
public:

    LdapException(const QString& m) : _message(m) {  }

    QString getMessage() {
        return _message;
    }
};

#endif	/* _MYLDAP_H */

