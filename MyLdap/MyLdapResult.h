/* 
 * File:   MyLdapResult.h
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

#ifndef _MYLDAPRESULT_H
#define	_MYLDAPRESULT_H

#define LDAP_DEPRECATED 1
#include <ldap.h>

#include "MyLdap.h"
#include "Logger/Logger.h"
#include "MyLdapResultAttributes.h"

#include <QSharedPointer>

class MyLdap;
class MyLdapResultAttributes;

class MyLdapResult {
private:
    MyLdap* _ldap;
    LDAPMessage* _res;
    LDAPMessage* _actEntry;
public:
    MyLdapResult(MyLdap *ld, LDAPMessage *res);

    QSharedPointer<MyLdapResultAttributes> fetch();

    int countEntries();

    ~MyLdapResult();

};

#endif	/* _MYLDAPRESULT_H */

