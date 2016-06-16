#include "TcpAbstractCommandFactory.h"

// Konstruktor:
TcpAbstractCommandFactory::TcpAbstractCommandFactory(const QByteArray &groupName, const QByteArray &actionName)
    : _groupName(groupName), _actionName(actionName) {

}

// Destroy something...
TcpAbstractCommandFactory::~TcpAbstractCommandFactory() {

}
