#include "TcpClient.h"

TcpClient::TcpClient(QTcpSocket *c) : m_socket(c) {

}

TcpClient::~TcpClient() {

    // QTcpSocket zerst�ren, welches wir verwalten:
    delete m_socket;
}
