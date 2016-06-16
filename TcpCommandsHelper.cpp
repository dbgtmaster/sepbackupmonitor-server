#include "TcpCommandsHelper.h"

namespace TcpCommandsHelper {

    /**
      * Benutzer- Login
      **/
    QDataStream& operator<<(QDataStream &outStream, const userLogin &data) {
        return outStream << data.username << data.password;
    }

    QDataStream& operator>>(QDataStream &inStream, userLogin &data) {
        inStream >> data.username;
        inStream >> data.password;
        return inStream;
    }

    /**
      * Benutzer- Login - RESPONSE
      */
    QDataStream& operator<<(QDataStream &outStream, const userLoginResponse &data) {
        return outStream << data.success;
    }
    QDataStream& operator>>(QDataStream &inStream, userLoginResponse &data) {
        inStream >> data.success;
        return inStream;
    }

    /**
      * LDAP- Filter:
      */
    QDataStream& operator<<(QDataStream &outStream, const ldapFilter &data) {
        return outStream << data.baseDn << data.filter << static_cast<qint32>(data.scope);
    }
    QDataStream& operator>>(QDataStream &inStream, ldapFilter &data) {
        inStream >> data.baseDn;
        inStream >> data.filter;
        int tmp;
        inStream >> tmp;
        data.scope = static_cast<ldapScope>(tmp);
        return inStream;
    }

    /**
      * Eine Benutzergruppe:
      */
    QDataStream& operator<<(QDataStream &outStream, const userGroup &data) {
        return outStream << data.name << data.locked << data.ldapFilter1Enabled << data.ldapFilter2Enabled << data.ldapFilter3Enabled
                            << data.ldapFilter1 << data.ldapFilter2 << data.ldapFilter3;
    }
    QDataStream& operator>>(QDataStream &inStream, userGroup &data) {
        inStream >> data.name;
        inStream >> data.locked;
        inStream >> data.ldapFilter1Enabled;
        inStream >> data.ldapFilter2Enabled;
        inStream >> data.ldapFilter3Enabled;
        inStream >> data.ldapFilter1;
        inStream >> data.ldapFilter2;
        inStream >> data.ldapFilter3;

        return inStream;
    }

}
