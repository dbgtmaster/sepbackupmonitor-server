/* 
 * File:   MyLdapResultAttributes.cpp
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
 * Created on 21. Juli 2010, 15:24
 */

#include "MyLdapResultAttributes.h"

#include <QString>

MyLdapResultAttributes::MyLdapResultAttributes(MyLdap *ld, LDAPMessage *res) {

    _res = res;
    _ld = ld;
    _ptr = 0;
    _lastAttr = 0;

    logDebug("MyLdapResultAttributes::MyLdapResultAttributes(): Entry: '%1'", getDn());
}

QString MyLdapResultAttributes::getDn() {

    return _ld->getEntryDn(_res);
}

char* MyLdapResultAttributes::fetch() {

    if (_lastAttr) _ld->memFree(_lastAttr);

    if (_ptr == 0) {

        // Der erste Eintrag:
        logDebug("MyLdapResultAttributes::fetch(): Get first attribute.");
        _lastAttr = _ld->firstAttribute(_res, &_ptr);
    }
    else {

        // Nächsten Eintrag:
        logDebug("MyLdapResultAttributes::fetch(): Get next attribute.");
        _lastAttr = _ld->nextAttribute(_res, _ptr);
    }

    // Wenn _actEntry NULL ist, so sind wir alle Einträge durch...
    if (_lastAttr == 0) {
        logDebug("MyLdapResultAttributes::fetch: No further attribute available.");
        return 0;
    }

    logDebug("MyLdapResultAttributes::fetch(): Get Attribute values '%1'", _lastAttr);

    return _lastAttr;
}

QSharedPointer< QList<QString> > MyLdapResultAttributes::getValues() {

    // Werte dieses Attributes ermitteln:
    return getValues(_lastAttr);
}

QSharedPointer< QList<QString> > MyLdapResultAttributes::getValues(char *attribute) {

    logDebug("MyLdapResultAttributes::getValues(char *attribute): Get attribute values '%1'", attribute);

    char** r = _ld->getAttrValues(_res, attribute);

    // Alle werde in eine QList speichern...
    QList<QString>* list = new QList<QString>();

    int count = _ld->countValues(r);
    logDebug("MyLdapResultAttributes::getValues(char *attribute): Count value entries: %1", QString::number(count));
    for (int i = 0; i < count; i++) {
        list->append( r[i] );
        logDebug("MyLdapResultAttributes::getValues(char *attribute): fetched value entry %1/%2",
                 QString::number(i + 1), QString::number(count) );
    }

    _ld->valueFree(r);

    return QSharedPointer< QList<QString> >(list);
}

MyLdapResultAttributes::~MyLdapResultAttributes() {

    if (_lastAttr) {
        _ld->memFree(_lastAttr);
        _lastAttr = 0;
    }
    if (_ptr) {
        _ld->berFree(_ptr);
        _ptr = 0;
    }
}
