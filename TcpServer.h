#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork/QTcpServer>

#include <TcpSocketThread.h>

#include <Logger/Logger.h>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

    // ... um Anfragen in einem eigenen Thrad zu starten:
    void incomingConnection(int handle);
signals:

public slots:

};

#endif // TCPSERVER_H
