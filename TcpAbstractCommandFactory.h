#ifndef TCPABSTRACTCOMMANDFACTORY_H
#define TCPABSTRACTCOMMANDFACTORY_H

#include <QByteArray>

class TcpAbstractCommand;

class TcpAbstractCommandFactory
{
protected:
    QByteArray _groupName;
    QByteArray _actionName;

public:
    explicit TcpAbstractCommandFactory(const QByteArray &groupName, const QByteArray &actionName);
    virtual ~TcpAbstractCommandFactory();

    // inline methods:
    const QByteArray& getGroupName() const;
    const QByteArray& getActionName() const;

    // Gibt ein neues Objekt des Kommands zurück...
    virtual TcpAbstractCommand* createCommand() = 0;
};

inline const QByteArray& TcpAbstractCommandFactory::getActionName() const  {
    return _actionName;
}


inline const QByteArray& TcpAbstractCommandFactory::getGroupName() const  {
    return _groupName;
}

#endif // TCPABSTRACTCOMMANDFACTORY_H
