#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

// Wenn nach X Sekunden keine Aktivit�t vom Client erfolgt, wird die Verbindung getrennt.
// Daher muss der Client innerhalb des Zeitraumes immer mit main/timeoutTimer die Zeit zur�cksetzen..
// Angabe in Millisekunden!
#define SOCKET_TIMEOUT 300000

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>

#include "dbModels/dbUserModel.h"

class TcpSocketThread : public QThread
{
    Q_OBJECT

private:
    QTcpSocket _socket;     // QTcpSocket;

    quint32 _socketBlockSize;           // Wie viel lesende Daten sind zu erwarten, bis der
                                        // angeforderte Befehl ausgef�hrt werden kann?
                                        // Wenn auf 0 gesetzt, so gibt es derzeit nichts
                                        // zum lesen oder noch nicht gen�gend Daten erhalten,
                                        // um die blockSize zu bestimmen...

    // Cache, in welchem die erhaltenen Daten des Sockets
    // zwischengespeichert werden, bis alle Daten vom Client
    // �bertragen wurden.
    QByteArray m_socketReadCache;
    quint32 m_blockSize;      // Anzahl der Bytes, welche vom Clients empfangen werden...

    // Model des eingeloggten Benutzers:
    dbUserModel _user;

    QTimer *_timeoutTimer;

public:
    TcpSocketThread(int descriptor, QObject *parent = 0);

    // Die Threadausf�hrung
    void run();

    // Socket schlie�en, und somit diesen Thread beenden...
    void close();

public slots:

    // Wenn neue Daten zum lesen vorhanden sind:
    void newDataToRead();

    // �berladen, um in Log- Datei schreiben zu k�nnen, das Thread zerst�rt wird...
    void deleteLater();

    // Wenn ein Fehler mit der TCP Verbindung aufgetreten ist:
    void socketError(QAbstractSocket::SocketError);

    void checkState();

    void socketTimeout();
};

#endif // TCPSOCKETTHREAD_H
