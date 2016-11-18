#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{

}

// Methode wird aufgerufen, wenn eine neue Eingehende Verbindung
// verfügbar ist.
void TcpServer::incomingConnection(qintptr handle) {

    logNotice("New incoming connection...");

    // Neuen Thread erstellen...
    logDebug("Creates new thread for the new connection.");
    TcpSocketThread *thread = new TcpSocketThread(handle);

    // Wenn Thread beendet wurde, so die Ressourcen des Threads freigeben...
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // ... Und Action!
    logDebug("Start the new thread for the new connection.");
    thread->start();
}
