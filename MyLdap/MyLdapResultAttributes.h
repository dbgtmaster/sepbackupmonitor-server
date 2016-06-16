/* 
 * File:   MyLdapResultAttributes.h
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

#ifndef MYLDAPRESULTATTRIBUTES_H
#define	MYLDAPRESULTATTRIBUTES_H

#define LDAP_DEPRECATED 1
#include <ldap.h>

#include "MyLdap.h"
#include "Logger/Logger.h"
#include <QList>

class MyLdap;
class QByteArray;

class MyLdapResultAttributes {
private:
    MyLdap *_ld;
    LDAPMessage* _res;
    BerElement* _ptr;
    char* _lastAttr;

public:
    MyLdapResultAttributes(MyLdap *ld, LDAPMessage *res);
    ~MyLdapResultAttributes();

    char* fetch();
    QSharedPointer< QList<QString> > getValues();
    QSharedPointer< QList<QString> > getValues(char *attribute);

    QString getDn();
};

#endif	/* MYLDAPRESULTATTRIBUTES_H */

