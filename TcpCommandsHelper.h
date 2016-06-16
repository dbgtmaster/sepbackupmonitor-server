#ifndef TCPCOMMANDSHELPER_H
#define TCPCOMMANDSHELPER_H

#include <QString>
#include <QDataStream>

namespace TcpCommandsHelper {

    /**
      * Bestimmt bei der Antwort des Servers, ob es sich um eine normale Antwort handelt, oder ob
      * zB ein Fehler aufgetreten ist (nicht ausreichend Rechte).
      * Dieses Bit befindet sich nach der commId.
      */
    enum responseType {
        NORMAL_RESPONSE,        // Es ist kein Fehler aufgetreten, somit die erwartete Antwort...
        RIGHT_ERROR             // Es ist ein Rechte- Problem aufgetreten...
    };

    enum Action {
        CREATE,
        MODIFY,
        UNKNOWN
    };

    /**
      * Benutzer- Authentifizieren:
      * Zum senden der Logindaten
      */
    struct userLogin {
        QString username;
        QString password;
    };
    QDataStream& operator<<(QDataStream &outStream, const userLogin &data);
    QDataStream& operator>>(QDataStream &inStream, userLogin &data);

    /**
      * Benutzer- Authentifizierung RESPONSE:
      * Antwort des Servers, ob Authentifizierung erfolgreich war,
      * sowie sämtliche Benutzer- Daten
      */
    struct userLoginResponse {
       bool success;                // Ob Login erfolgreich war.
    };
    QDataStream& operator<<(QDataStream &outStream, const userLoginResponse &data);
    QDataStream& operator>>(QDataStream &inStream, userLoginResponse &data);

    /**
      * Definiert einen LDAP- Filter:
      */
    enum ldapScope { LDAP_BASE, LDAP_ONE, LDAP_SUB };
    struct ldapFilter {
        QString baseDn;
        ldapScope scope;
        QString filter;
    };
    QDataStream& operator<<(QDataStream &outStream, const ldapFilter &data);
    QDataStream& operator>>(QDataStream &inStream, ldapFilter &data);

    /**
      * Stellt eine Usergruppe dar:
      */
    struct userGroup {
        QString name;
        bool locked;
        ldapFilter ldapFilter1;
        bool ldapFilter1Enabled;
        ldapFilter ldapFilter2;
        bool ldapFilter2Enabled;
        ldapFilter ldapFilter3;
        bool ldapFilter3Enabled;
    };
}

#endif // TCPCOMMANDSHELPER_H
