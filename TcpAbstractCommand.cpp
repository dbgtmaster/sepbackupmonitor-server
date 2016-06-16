#include "TcpAbstractCommand.h"

#include "dbModels/dbUserModel.h"

TcpAbstractCommand::TcpAbstractCommand() : _dataStreamWrite(&_dataStreamWriteArray, QIODevice::WriteOnly), _db(DatabasePool::get()), _dataStreamRead(0)
{

    /**
      * @TODO:
      * Da der TCP- Server derzeit mit 4.6 läuft. Nach QT eventuell versuchen, Version hochzusetzen...
      */
    _dataStreamWrite.setVersion(QDataStream::Qt_4_6);

    // Schreibenden Datenstream initialisieren:
    initDataStreamWrite();
}

TcpAbstractCommand::~TcpAbstractCommand() {

}

// Den schreibenden Datenstream reseten und initialiseren:
void TcpAbstractCommand::initDataStreamWrite() {

    _dataStreamWriteArray.clear();

    // Anzahl der Gesamt- zu übertragenden Daten:
    _dataStreamWrite << (quint32) 0;

    // Die commID, damit der Client weiß, von welcher Anfrage dies die Antwort ist:
    _dataStreamWrite << (quint16) 0;

    // Dann wird bestimmt, ob es sich um eine normale Antwort handelt oder etwa eine Fehlermeldung für ein Rechteproblem...
    // wird mit TcpCommandsHelper::responseType gesetzt.
    _dataStreamWrite << (quint32) 0;
}

void TcpAbstractCommand::setSocket(QTcpSocket *s) {
    _socket = s;
}


void TcpAbstractCommand::setCommId(quint16 commId){
    _commId = commId;
}

void TcpAbstractCommand::setDataStreamRead(QDataStream *a) {
    _dataStreamRead = a;

    /**
      * @TODO:
      * Da der TCP- Server derzeit mit 4.6 läuft. Nach QT eventuell versuchen, Version hochzusetzen...
      */
    _dataStreamRead->setVersion(QDataStream::Qt_4_6);
}

void TcpAbstractCommand::setUserAccount(dbUserModel *u) {
    _user = u;
}

void TcpAbstractCommand::writeDataStream(TCP_RESPONSE_TYPE t) {

    // Nun füllen wir die ganzen vordefinierten Platzhalter im Datenstream aus:

    // Gesamtgröße des Streams:
    _dataStreamWrite.device()->seek(0);
    _dataStreamWrite << (quint32)(_dataStreamWriteArray.size() - sizeof(quint32) );

    // Die commId der Kommunikation:
    _dataStreamWrite << (quint16) _commId;

    // Und den Typ der Antwort (ob es sich um eine normale Antwort handelt, oder etwa ein Fehler aufgetreten ist (Rechteproblem,...)
    _dataStreamWrite << (quint32) t;

    // Daten über Socket senden:
    _socket->write(_dataStreamWriteArray);
    _socket->flush();
}

void TcpAbstractCommand::setCommandDetails(QByteArray group, QByteArray name) {
    _commandDetails_group = group;
    _commandDetails_name = name;
}

QByteArray TcpAbstractCommand::group() {
    return _commandDetails_group;
}

QByteArray TcpAbstractCommand::command() {
    return _commandDetails_name;
}

void TcpAbstractCommand::postExec(const QVariant &var) {

    /**
      * Kann überladen werden ...
      */
}
