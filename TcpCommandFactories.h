#ifndef TCPCOMMANDFACTORIES_H
#define TCPCOMMANDFACTORIES_H

#include <TcpAbstractCommandFactory.h>

#include <map>

#include <QByteArray>
#include <QMultiHash>

typedef std::map<const QByteArray, TcpAbstractCommandFactory* > factoryActionMap;
typedef std::map<const QByteArray, factoryActionMap > factoryGroupMap;

class TcpAbstractCommand;

class TcpCommandFactories
{
private:

    static TcpCommandFactories* _instance;

    // Stellt die einzelnen Gruppen (main, sys, backup,..), und als 2ten
    // Wert ein weiteres MAP mit den Aktionsnamen als QByteArray sowie das Factory dieser Aktion...
    factoryGroupMap _factoryMap;

    // Kann nur über ::instance() erreicht werden.
    explicit TcpCommandFactories();
    ~TcpCommandFactories();

public:

    static TcpCommandFactories* instance();

    // True, wenn hinzugefügt werden konnte, ansonsten false (wenn schon existiert).
    bool registerFactory(TcpAbstractCommandFactory* factory);

    // gibt eine Liste mit den Namen aller registrierten Factories zurück:
    // Format: multiHash[groupName] = actionName;
    QMultiHash<QByteArray, QByteArray> getFactories();

    // Gibt einen Befehl zurück:
    // Wenn "0" zurückgegeben wird, konnte der Befehl nicht ermittelt werden...
    TcpAbstractCommand* createCommand(const QByteArray& group, const QByteArray& action) const;
};

#endif // TCPCOMMANDFACTORIES_H
