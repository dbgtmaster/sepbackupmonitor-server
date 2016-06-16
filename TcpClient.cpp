#include "TcpClient.h"

TcpClient::TcpClient(QTcpSocket *c) : m_socket(c) {

}

TcpClient::~TcpClient() {

    // QTcpSocket zerstören, welches wir verwalten:
    delete m_socket;
}
