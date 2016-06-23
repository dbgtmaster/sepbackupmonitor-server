/* 
 * File:   MyLdap.cpp
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

#include "MyLdap.h"

#include <QString>

#include <Logger/Logger.h>
#include "Config.h"

#include "TcpCommandsHelper.h"

MyLdap* MyLdap::_instance = 0;

MyLdap::MyLdap() : _isConnected(false) {

    // Open ldap connection:
    logNotice("Creates MyLdap::MyLdap() object...");
}

MyLdap* MyLdap::instance() {

    if (_instance == 0) {
        _instance = new MyLdap;
    }

    return _instance;
}

void MyLdap::deleteInstance() {

    if (_instance) {
        delete _instance;
        _instance = 0;
    }
}

void MyLdap::connect(const QString &hostname) {

    _ld = ldap_init(hostname.toUtf8().data(), 389);
    _hostname = hostname;
}

void MyLdap::connectByConfig() {
    connect( Config::instance()->get("auth_backend_ldap", "address") );
}

bool MyLdap::bind(const QString& bindDn, const QString& password) {

    // Benutzer & PW zwischenspeichern, damit bei einem Fehler ein erneuter
    // Bind durchgeführt werden kann...
    _bindDn = bindDn,
    _password = password;

    int ldapVersion = LDAP_VERSION3;
    ldap_set_option(_ld, LDAP_OPT_PROTOCOL_VERSION, &ldapVersion);

    // Am LDAP Server binden:
    logNotice("Bind with ldap-DN '%1'.", bindDn);

    int authType;
    if (bindDn.count() == 0) {
        authType = LDAP_AUTH_SIMPLE;
    }
    else {
        authType = LDAP_AUTH_SIMPLE;
        logNotice("No binddn given, binding anonym...");
    }
    if ( int rc = ldap_bind_s(_ld, bindDn.toUtf8().data(), password.toUtf8().data(), authType) ) {

        // Verbindung zum LDAP Server fehlgeschlagen:
        logFatal("MyLdap::bind(): Bind to LDAP-host '%1', DN '%2' failed. Error: %3", _hostname, bindDn, err2String(rc));
        return false;
    }

    _isConnected = true;
    return true;
}

bool MyLdap::bindByConfig() {

    return bind( Config::instance()->get("auth_backend_ldap", "bind_dn"),
                 Config::instance()->get("auth_backend_ldap", "bind_password") );
}

void MyLdap::unbind() {
    logDebug("MyLdap::unbind(): Close connection to the ldap server...");
    ldap_unbind_s(_ld);
}

QSharedPointer<MyLdapResult> MyLdap::search(const QString& dn, int scope, const QString& filter) {

    logNotice("Run ldapsearch: dn: '%1', searchScope: '%2', filter: '%3'", dn, QString::number(scope), filter);
    LDAPMessage *res;    // Ergebnis
    int rc = ldap_search_s(_ld, dn.toUtf8().data(), scope, filter.toUtf8().data(), NULL, 0, &res );

    MyLdapResult* r = new MyLdapResult(this, res);

    // War erfolgreich??
    if (rc != LDAP_SUCCESS) {

        logFatal("MyLdap::search(): LDAP search failed! DN '%1', filter '%2' error: '%3'", dn, filter, err2String(rc));
        return QSharedPointer<MyLdapResult>(0);
    }

    return QSharedPointer<MyLdapResult>(r);
}

QSharedPointer<MyLdapResult> MyLdap::search(const TcpCommandsHelper::ldapFilter &filter) {

    int searchScope;
    if (filter.scope == TcpCommandsHelper::LDAP_ONE) {
        searchScope = LDAP_SEARCH_ONE;
    }
    else if (filter.scope == TcpCommandsHelper::LDAP_BASE) {
        searchScope = LDAP_SEARCH_BASE;
    }
    else {
        searchScope = LDAP_SEARCH_SUB;
    }

    return search(filter.baseDn, searchScope, filter.filter);
}

LDAPMessage* MyLdap::firstEntry(LDAPMessage *res) {

    return ldap_first_entry(_ld, res);
}

LDAPMessage* MyLdap::nextEntry(LDAPMessage *res) {

    return ldap_next_entry(_ld, res);
}

QString MyLdap::getEntryDn(LDAPMessage *res) {

    char *dn =  ldap_get_dn(_ld, res);
    QString sdn(dn);
    memFree(dn);

    return sdn;
}

char* MyLdap::firstAttribute(LDAPMessage *res, BerElement **ptr) {

    return ldap_first_attribute(_ld, res, ptr);
}

char* MyLdap::nextAttribute(LDAPMessage *res, BerElement *ptr) {

    return ldap_next_attribute(_ld, res, ptr);
}

char** MyLdap::getAttrValues(LDAPMessage *res, char *attribute) {

    char **v;
    v = ldap_get_values(_ld, res, attribute);

    // Wenn Wert "0" ist, so existiert dieses Attribute beim Suchergebnis nicht...
    if (v == 0) {
        logFatal("LDAPMessage has no attribute '%1'!", QString(attribute) );
    }

    return v;
}

int MyLdap::countValues(char** values) {

    return ldap_count_values(values);
}

int MyLdap::countEntries(LDAPMessage *res) {

   return ldap_count_entries(_ld, res);
}

void MyLdap::memFree(char* dn) {
    ldap_memfree( dn );
}

void MyLdap::berFree(BerElement *ber){
    ber_free(ber, 0);
}

void MyLdap::valueFree(char **vals) {
    ldap_value_free(vals);
}
void MyLdap::msgFree(LDAPMessage *msg) {
    ldap_msgfree(msg);
}

void MyLdap::modsFree(LDAPMod **mod) {
    ldap_mods_free(mod, 1);
}

/**
 * Escaped einen String, um ihn sorgenfrei in einem Filter benutzen zu können...
 */
QString MyLdap::escapeSearchFilter(const QString &toEscape) {

   QString escaped;
   logDebug(toEscape);
   // Alle Zeichen einzeln durchlaufen & gegebenenfalls ersetzen...
   int charCount = toEscape.size();
   for (int i = 0; i < charCount; i++) {
       logDebug(toEscape.at(i));
       switch (toEscape.at(i)) {
           case '\\':
               escaped += "\\\\";
               break;
           case '*':
               escaped += "\\*";
               break;
           case '(':
               escaped += "\\(";
               break;
           case ')':
               escaped += "\\)";
               break;
           case '/':
               escaped += "\\/";
               break;
           default:
           escaped += toEscape.at(i);
       }
   }

   return escaped;
}

/**
 * Escaped einen String, um ihn sorgenfrei in einer DN benutzen zu können...
 */
QString MyLdap::escapeDn(const QString& toEscape) {

   QString escaped;

   // Alle Zeichen einzeln durchlaufen & gegenfalls ersetzen...
   int charCount = toEscape.size();
   for (int i = 1; i <= charCount; i++) {

       switch (toEscape.at(i).toUtf8()) {
           case '\\':
               escaped += "\\\\";
               break;
           case ',':
               escaped += "\\,";
               break;
           case '+':
               escaped += "\\+";
               break;
           case '"':
               escaped += "\\\"";
               break;
           case '<':
               escaped += "\\<";
               break;
           case '>':
               escaped += "\\>";
               break;
           case ';':
               escaped += "\\;";
               break;
           default:
           escaped += toEscape.at(i);
       }
   }

   return escaped;
}

QString MyLdap::err2String(int errCode) {

    return QString( ldap_err2string(errCode) );
}

MyLdap::~MyLdap() {
    unbind();
}
