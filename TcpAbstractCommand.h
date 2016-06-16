#ifndef TCPABSTRACTCOMMAND_H
#define TCPABSTRACTCOMMAND_H

#include <QtCore>
#include <QTcpSocket>

#include "Utils/UtilDataStream.h"

#include "Database/DatabasePool.h"

#include "TcpCommandsHelper.h"

class Database;
class dbUserModel;

class TcpAbstractCommand
{
    // Damit der Socket die Daten
    friend class TcpSocketThread;
private:

    // In welcher Gruppe befindet sich dieses Element?
    // z.B.: "main", "backupmonitor", "timerecording", ...
    QByteArray _commandDetails_group;

    // Name dieses Befehls
    QByteArray _commandDetails_name;

    // KommunikationsID
    quint16 _commId;

    // Stream mit dem die Daten geschrieben werden:
    // Benutzt hierfür werden können beide Eigenschaften, da diese miteinander
    // vom Konstruktor verbunden werden...
    // Reihenfolge dieser 2 Membern nicht ändern!
    QByteArray _dataStreamWriteArray;
    QDataStream _dataStreamWrite;

    // Stream, mit welchem die Daten gelesen werden...
    // Wird von der static setSocket Methode gesetzt...
    QDataStream *_dataStreamRead;

    // Tcp Socket:
    QTcpSocket* _socket;

protected:

    // Verbindung zur Datenbank (wird via DatabasePool::get() geholt).
    DB _db;

    // Eingeloggter Benutzer:
    dbUserModel* _user;

    // Reset & initialisiert den schreibenden Datenstream:
    void initDataStreamWrite();

    // Liest Daten vom Datenstream:
    template <typename DATA_TYPE>
    inline void _streamRead(DATA_TYPE &var);
    template <typename DATA_TYPE>
    inline void _streamRead(DATA_TYPE &var, DATA_TYPE &var2);

    // Wenn der lesende Stream das Ende erreicht hat
    inline bool _streamReadAtAnd();

    // Schreibt Daten in den Datenstream:
    template <typename DATA_TYPE>
    inline void _streamWrite(const DATA_TYPE &var);
    template <typename DATA_TYPE>
    inline void _streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2);
    template <typename DATA_TYPE>
    inline void _streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3);
    template <typename DATA_TYPE>
    inline void _streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3, const DATA_TYPE &var4);
    template <typename DATA_TYPE>
    inline void _streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3, const DATA_TYPE &var4, const DATA_TYPE &var5);
    template <typename DATA_TYPE>
    inline void _streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3, const DATA_TYPE &var4, const DATA_TYPE &var5, const DATA_TYPE &var6);
    inline void _streamWriteClear();


public:

    explicit TcpAbstractCommand();
    virtual ~TcpAbstractCommand();

    // Setzt die Gruppe dieser Aktion... (main, backupmonitor,...) und Befehlsname
    void setCommandDetails(QByteArray group, QByteArray name);
    QByteArray group();
    QByteArray command();

    // Setzt die KommunikationsID, welche dem Client übertragen wird,
    // damit dieser zuordnen kann, von welcher Anfrage dies die Antwort ist...
    void setCommId(quint16 commId);

    // Setzt den Socket, von welchem gelesen werden soll...
    void setSocket(QTcpSocket *s);

    // QByteArray, von welchem die Daten gelesen werden sollen, welche vom Client empfangen wurden.
    void setDataStreamRead(QDataStream *);

    // Ob es sich um eine normale Antwort handelt, oder einen Rechtefehler,...?
    void setResponseType(TcpCommandsHelper::responseType type);

    // Um den eingeloggten Account zu übergeben...
    void setUserAccount(dbUserModel *u);

    // Muss überschrieben werden, hier spielen sich die eigentlichen Dinge des Befehls ab...
    virtual QVariant exec() = 0;
    virtual void postExec(const QVariant &var);        // Die verarbeiteten Daten von exec() lossenden...
                                    // Kann überladen werden, muss aber nicht, die Standardmethode
                                    // ist leer...

    // Die Art des Antworts, welche zum Client gesendet wird:
    enum TCP_RESPONSE_TYPE {
        NORMAL_RESPONSE = 0,              // Es werden die gewollten Daten zum Client gesendet.
        TCP_PERMISSION_ERROR = 1        // Es wird ein Tcp- Berechtigungsfehler zum Client gesendet.
    };
    // Schreibt den WriteStream in den Tcp- Datenstrom zum Client:
    void writeDataStream(TCP_RESPONSE_TYPE t);

    // Trennt die Verbindung zum Gesrpächspartner
    inline void closeSocket();
};



template<typename DATA_TYPE>
void TcpAbstractCommand::_streamRead(DATA_TYPE &var) {
    UtilDataStream::read(*_dataStreamRead, var);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamWrite(const DATA_TYPE &var) {
    UtilDataStream::write(_dataStreamWrite, var);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamRead(DATA_TYPE &var, DATA_TYPE &var2) {
    UtilDataStream::read(*_dataStreamRead, var);
    UtilDataStream::read(*_dataStreamRead, var2);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2) {
    UtilDataStream::write(_dataStreamWrite, var);
    UtilDataStream::write(_dataStreamWrite, var2);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3) {
    UtilDataStream::write(_dataStreamWrite, var);
    UtilDataStream::write(_dataStreamWrite, var2);
    UtilDataStream::write(_dataStreamWrite, var3);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3, const DATA_TYPE &var4) {
    UtilDataStream::write(_dataStreamWrite, var);
    UtilDataStream::write(_dataStreamWrite, var2);
    UtilDataStream::write(_dataStreamWrite, var3);
    UtilDataStream::write(_dataStreamWrite, var4);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3, const DATA_TYPE &var4, const DATA_TYPE &var5) {
    UtilDataStream::write(_dataStreamWrite, var);
    UtilDataStream::write(_dataStreamWrite, var2);
    UtilDataStream::write(_dataStreamWrite, var3);
    UtilDataStream::write(_dataStreamWrite, var4);
    UtilDataStream::write(_dataStreamWrite, var5);
}

template<typename DATA_TYPE>
void TcpAbstractCommand::_streamWrite(const DATA_TYPE &var, const DATA_TYPE &var2, const DATA_TYPE &var3, const DATA_TYPE &var4, const DATA_TYPE &var5, const DATA_TYPE &var6) {
    UtilDataStream::write(_dataStreamWrite, var);
    UtilDataStream::write(_dataStreamWrite, var2);
    UtilDataStream::write(_dataStreamWrite, var3);
    UtilDataStream::write(_dataStreamWrite, var4);
    UtilDataStream::write(_dataStreamWrite, var5);
    UtilDataStream::write(_dataStreamWrite, var6);
}

void TcpAbstractCommand::_streamWriteClear() {
    _dataStreamWriteArray.clear();
}

void TcpAbstractCommand::closeSocket() {
    _socket->close();
}

bool TcpAbstractCommand::_streamReadAtAnd() {
    return _dataStreamRead->atEnd();
}

#endif // TCPABSTRACTCOMMAND_H
