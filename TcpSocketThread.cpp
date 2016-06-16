 #include <TcpSocketThread.h>

#include <auto_ptr.h>

#include <QApplication>
#include <QHostAddress>
#include <QTime>

#include <TcpCommands/main/userLogin.h>
#include <TcpCommands/main/serverVersion.h>

#include <Logger/Logger.h>

#include <TcpCommandFactories.h>

#include <dbModels/dbAbstractModel.h>

#include "Database/DatabasePool.h"

#include <Config.h>

#include "Utils/UtilUsers.h"
#include "Utils/UtilDataStream.h"

TcpSocketThread::TcpSocketThread(int descriptor, QObject *parent) : QThread(parent)
{

    // Sich selber in den Thread verschieben...
    moveToThread(this);

    // Socket erstellen und in den Thread verschrieben...
    if ( ! _socket.setSocketDescriptor(descriptor) ) {
        logError("Could not set socket descriptor for the new connection. Disconnect client.");
        _socket.close();
    }
    _socket.setParent(NULL);
    _socket.moveToThread(this);

}

void TcpSocketThread::checkState() {

    qDebug() << _socket.state();
}

void TcpSocketThread::run() {

    _timeoutTimer = new QTimer(this);
    _timeoutTimer->moveToThread(this);
    _timeoutTimer->setInterval(SOCKET_TIMEOUT);
    connect(_timeoutTimer, SIGNAL(timeout()), this, SLOT(socketTimeout()));
    _timeoutTimer->start(SOCKET_TIMEOUT);

    // Ist noch kein Socket Deskriptor gesetzt, so ist beim setzen ein
    // Fehler aufgetreten, und wir beenden diesen Thread...
    if (_socket.socketDescriptor() == -1) {
        logError("Socket descriptor for this socket not set. Exit tcp thread...");
        return;
    }

    // IP Adresse & Port loggen...
    logNotice( QString("Client connection from IP %1, port %2, to port %3")
                    .arg(_socket.localAddress().toString(),
                         QString::number( _socket.localPort() ),
                         QString::number( _socket.peerPort() )
                         )
               );

    logDebug("Thread for the connection started.");

    logDebug("Install eventhandler, to close this thread, when tcp connection closed.");
    connect(&_socket, SIGNAL(disconnected()), this, SLOT(quit()));
    connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    // Wenn neue Daten zum lesen vorhanden sind:
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(newDataToRead()));

    // Datenstream auf Socket setzen:
    _socketBlockSize = 0;

    /**
      * Nun fangen wir an, am Tcp Port zu lauschen...
      */
    logDebug("And now, i am waiting for data to read...");

    exec();
}

void TcpSocketThread::newDataToRead() {

    QTime timeStart( QTime::currentTime() );

    try {

        // Die zu verarbeitenden Daten, welche vom Client zum Server gesendet wurden...
        QByteArray clientData;

        logDebug("New Data to read available.");

        // Wurde schon bestimmt, wie viele Bytes vom Client erwarten zu sind?
        if (_socketBlockSize == 0) {

            logDebug("Try to detect the size of the incoming data...");

            // Wurden schon genug Daten empfangen, um die zu erwartenden Block zu bestimmen?
            if (_socket.bytesAvailable() <= sizeof(quint32) ) {
                // Wir müssen auf mehr Daten des Clients warten...
                logDebug("Not enough data arrived. I wait for more data...");
                return;
            }

            // Anzahl erwartende Bytes:
            QDataStream dataStream(&_socket);
            dataStream >> _socketBlockSize;
            logDebug( QString("Size of incoming data detected. Size are %1 bits").arg(QString::number(_socketBlockSize)) );
        }

        // Wurden vom Client bereits alle Daten empfangen???
        if (_socket.bytesAvailable() != _socketBlockSize) {

            logDebug("Not all data packages from the client arrived (%1 from %2 bytes) Wait for more data...",
                     QString::number(_socket.bytesAvailable()), QString::number(_socketBlockSize) );

            return;
        }
        else {

            logDebug("All data from the client arrived. Start parsing the data...");
            clientData = _socket.read(_socketBlockSize);

        }

        QDataStream dataStreamRead(&clientData, QIODevice::ReadWrite);

        /**
          * Alle Daten vom Client empfangen, jetzt folgt die Bearbeitung der
          * empfangenen Daten
          */

        logDebug("Start extract the incoming datas...");

        // Art des Befehls:
        // 0 -> TcpCommand group/command
        // 1 -> timeout reseter
        quint8 tcpType;
        UtilDataStream::read(dataStreamRead, tcpType);
        switch (tcpType) {
            case 0:

                /**
                  * Tcp Command:
                  */
            {
                // Kommunikations- ID:
                quint16 commId;
                UtilDataStream::read(dataStreamRead, commId);

                // Gruppenname, in welcher sich diese Aktion befindet.
                QByteArray group;
                UtilDataStream::read(dataStreamRead, group);

                // Name der auszuführenden Aktion
                QByteArray action;
                UtilDataStream::read(dataStreamRead, action);


                logNotice("Try to find action %1/%2...", group, action);

                std::auto_ptr<TcpAbstractCommand> command( TcpCommandFactories::instance()->createCommand(group, action) );
                // Wurde 0 zurückgegeben, so existiert der Befehl nicht...
                if (command.get() == 0) {
                    logError("Could not run tcp command '%1/%2'. Action does not exists!", group, action);
                    close();
                    return;
                }

                command->setCommId(commId);
                command->setSocket(&_socket);
                command->setUserAccount(&_user);
                command->setDataStreamRead(&dataStreamRead);

                /**
                  * Überprüfen, ob Befehl ausgeführt werden darf...
                  */
                TcpAbstractCommand::TCP_RESPONSE_TYPE answerType = TcpAbstractCommand::NORMAL_RESPONSE;
                // Hat User die Berechtigungen, diesen Befehl auszuführen??
                if (! UtilUsers::canRunTcpCommand(_user.getColumn("id").toInt(), group, action)  ) {

                    /**
                      * Benutzer hat nicht entsprechende Berechtigungen, um diesen Befehl auszuführen:
                      */

                    logError("User has not enough permissions to run the tcp action %1/%2", group, action);
                    answerType = TcpAbstractCommand::TCP_PERMISSION_ERROR;
                }
                else {

                    // Befehl ausführen:
                    logDebug("Run action %1/%2...", group, action);
                    QVariant execReturn = command->exec();

                    logDebug("Run postExec().");
                    command->postExec( execReturn );
                }

                logDebug("Tramsit data over the tcp socket to the client...");
                command->writeDataStream(answerType);
                logDebug("Datastream transmitted success!...");

                break;
            }
            case 1:
            {
                /**
                  * Timeout Reseter:
                  */

                logDebug("Reset socket timeout.");

                // wir schreiben den Client die Zahl "15253" zurück...
                QByteArray dataStreamWriteArray;
                QDataStream dataStreamWrite(&dataStreamWriteArray, QIODevice::WriteOnly);
                quint16 answer = 15253;
                dataStreamWrite << (quint32) sizeof(quint16);
                dataStreamWrite << answer;
                _socket.write(dataStreamWriteArray);
                _socket.flush();

                break;
            }
        default:
                logError("Unknown tcpType. Bye!");
                close();
                return;
        }

	logError("Tcp transction time: " + QString::number(timeStart.msecsTo(QTime::currentTime())) + "ms");

        _timeoutTimer->stop();
        _timeoutTimer->start(SOCKET_TIMEOUT);

    }
    catch (SqlException &e) {

        logFatal("Ein SQL Fehler ist aufgetreten!");
        logFatal("SQL: \n%1\n%2", e.sql(), e.error().databaseText());
        logFatal("Driver error: %1", e.error().driverText() );

        close();
    }
    catch (UtilDataStreamReadException &e) {
        logFatal("Error by reading tcp datastream data. close tcp connection.");
        logFatal("Error message is: %1", e.error());
    }
    catch (UtilDataStreamWriteException &e) {
        logFatal("Error by wrting tcp datastream data. close tcp connection.");
        logFatal("Error message is: %1", e.error());
    }

    // Empfangene Daten wurden nun abgearbeitet, daher in den Ursprungszustand
    // zurücksetzen und auf die nächsten Daten warten...
    _socketBlockSize = 0;
}

void TcpSocketThread::socketTimeout() {
    logWarning("Socket timeout " + QString::number(SOCKET_TIMEOUT) + " ms reached. Close connection.");
    close();
}

void TcpSocketThread::close() {

    // Schließt die Tcp Verbindung, und das werfende Signal "disconnected" der Tcp- Verbindung
    // beendet diesen Thread...
    _socket.close();
}

// Wird aufgerufen, wenn der Thread beendet wurde..
void TcpSocketThread::deleteLater() {

    // Müssen das Objekt zum Hauptthread verschieben, ansonstne erhalten wir ein Memory Leah
    moveToThread(QApplication::instance()->thread());

    logNotice("Client connection closed, destroy tcp socket thread...");

    // deleteLater der geerbten Klasse aufrufen, um Objekt zu zerstören...
    QThread::deleteLater();
}

void TcpSocketThread::socketError(QAbstractSocket::SocketError errId) {

    QString message;
    switch (errId) {
    case 0:
        message = "The connection was refused by the peer (or timed out).";
        break;
    case 1:
        message = "The remote host closed the connection.";
        break;
    case 2:
        message = "The host address was not found.";
        break;
    case 3:
        message = "The socket operation failed because the application lacked the required privileges.";
        break;
    case 4:
        message = "The local system ran out of resources (e.g., too many sockets).";
        break;
    case 5:
        message = "The socket operation timed out.";
        break;
    case 6:
        message = "The datagram was larger than the operating system's limit.";
        break;
    case 7:
        message = "An error occurred with the network (e.g., the network cable was accidentally plugged out).";
        break;
    // 8 and 9 only udp...
    case 10:
        message = "The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
        break;
    case 11:
        message = "Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
        break;
    case 12:
        message = "The socket is using a proxy, and the proxy requires authentication.";
        break;
    case 13:
        message = "The SSL/TLS handshake failed, so the connection was closed.";
        break;
    case 14:
        message = "Could not contact the proxy server because the connection to that server was denied.";
        break;
    case 15:
        message = "The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established).";
        break;
    case 16:
        message = "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
        break;
    case 17:
        message = "The proxy address was not found.";
        break;
    case 18:
        message = "The connection negotiation with the proxy server because the response from the proxy server could not be understood.";
        break;
    case -1:
        message = "An unidentified error occurred.";
        break;
    default:
        message = QString("TcpMainThread::emitTcpError(QString &message) have not defined the errorId QAbstractSocket::SocketError -> %1")
                            .arg( QString::number(errId) );
    }

    logNotice("TCP connection closed, reason: " + message);

    // Verbindung schließen:
    close();
}
