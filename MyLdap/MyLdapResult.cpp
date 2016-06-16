/* 
 * File:   MyLdapResult.cpp
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
 * Created on 20. Juli 2010, 17:30
 */

#include "MyLdapResult.h"

MyLdapResult::MyLdapResult(MyLdap *ldap, LDAPMessage *res) {

    _ldap = ldap;
    _res = res;
    _actEntry = 0;
}

QSharedPointer<MyLdapResultAttributes> MyLdapResult::fetch() {

    if (_actEntry == 0) {

        // Der erste Eintrag:
        logDebug("MyLdapResult::fetch: Get first entry.");
        _actEntry = _ldap->firstEntry( _res );
    }
    else {

        // Nächsten Eintrag:
        logDebug("MyLdapResult::fetch: Get next entry.");
        _actEntry = _ldap->nextEntry( _actEntry );
    }

    // Wenn _actEntry NULL ist, so sind wir alle Einträge durch...
    if (_actEntry == 0) {
        logDebug("MyLdapResult::fetch: No further entry available.");
        return QSharedPointer<MyLdapResultAttributes>(0);
    }

    return QSharedPointer<MyLdapResultAttributes>( new MyLdapResultAttributes(_ldap, _actEntry) );
}

int MyLdapResult::countEntries() {

    return _ldap->countEntries(_res);
}

MyLdapResult::~MyLdapResult() {

    if (_res) {
        _ldap->msgFree(_res);
        _res = 0;
    };
}
