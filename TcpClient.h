/**
  * Diese Klasse verwaltet die QTcpSockets, und speichert zu diesen Sockets
  * zusätzliche Informationen
  */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork/QTcpSocket>

class TcpClient
{
private:
    QTcpSocket* m_socket;

public:
    explicit TcpClient(QTcpSocket *s);
    ~TcpClient();
};

#endif // TCPCLIENT_H
