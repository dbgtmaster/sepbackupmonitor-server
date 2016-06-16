#include "TcpCommandFactories.h"

#include <Logger/Logger.h>

TcpCommandFactories* TcpCommandFactories::_instance = 0;

using namespace std;

TcpCommandFactories::TcpCommandFactories()
{
}

TcpCommandFactories* TcpCommandFactories::instance() {

    if (_instance == 0) {
        _instance = new TcpCommandFactories;
    }

    return _instance;
}

// add auf register unbenennen!
bool TcpCommandFactories::registerFactory(TcpAbstractCommandFactory* factory) {

    logDebug("TcpCommandFactories::addFactory: create Factory group '%1', action '%2'",
                factory->getGroupName(), factory->getActionName());

    // Nun legen wir den Gruppennamen an, in welcher sich dieser Factory liegt...
    // Sollte der Gruppennamen bereits exisiteren, so wird er einfach nicht angelegt.
    const QByteArray groupName = factory->getGroupName();
    pair<factoryGroupMap::iterator, bool> insertGroupState
            = _factoryMap.insert( pair<const QByteArray, factoryActionMap>( groupName, factoryActionMap() ) );

    // Nachdem die Gruppe nun existiert, legen wir in dieser Gruppe das Factory- Objekt ab:
    const QByteArray actionName = factory->getActionName();
    pair<factoryActionMap::iterator, bool> insertActionStatus
            = insertGroupState.first->second.insert( pair<const QByteArray, TcpAbstractCommandFactory*>(actionName, factory) );

    if (!insertActionStatus.second) {
        logFatal("TcpCommandFactories::addFactory: Factory already exists!!");
    }
    else {
        logDebug("TcpCommandFactories::addFactory: Factory created.");
    }

    // Gibt true zurück, wenn Factory angelegt wurde, ansonsten false, wenn ein Factory mit diesem Namen in dieser
    // Gruppe bereits existert...
    return insertActionStatus.second;
}

// Gibt den Befehl zurück:
// Wenn "0" zurückgegeben wird, konnte der Befehl nicht ermittelt werden...
TcpAbstractCommand* TcpCommandFactories::createCommand(const QByteArray& group, const QByteArray& action) const {

    logDebug("Run TcpCommandFactories::getFactory() with group '%1' and action '%2'", group, action);
    logDebug("Search group '%1' in the map...", group);

    factoryGroupMap::const_iterator itGroup;
    itGroup = _factoryMap.find(group);

    if (itGroup == _factoryMap.end()) {
        // Gruppe konnte nicht gefunden werden:

        logWarning("Get command '%1/%2'. Group does not exists!", group, action);

        return 0;
    }

    // Und nun nach der Aktion suchen:
    factoryActionMap::const_iterator itAction;
    itAction = itGroup->second.find(action);

    if (itAction == itGroup->second.end()) {
        // Aktion konnte nicht gefunden werden:

        logWarning("Get command '%1/%2'. Action does not exists!", group, action);

        return 0;
    }

    logWarning("Get command '%1/%2'. Successful!", group, action);

    // Alles gut gelaufen, geben wir den Befehl zurück:
    return itAction->second->createCommand();
}

QMultiHash<QByteArray, QByteArray> TcpCommandFactories::getFactories() {

    logDebug("Run TcpCommandFactories::getFactories().");

    // Liste, welche zurückgegeben wird...
    QMultiHash<QByteArray, QByteArray> hash;

    factoryGroupMap::iterator groupIT;
    for (groupIT = _factoryMap.begin(); groupIT != _factoryMap.end(); groupIT++) {

        factoryActionMap actionsMap = (*groupIT).second;

        factoryActionMap::iterator actionsIT;
        for (actionsIT = actionsMap.begin(); actionsIT != actionsMap.end(); actionsIT++) {

            hash.insert( (*groupIT).first, (*actionsIT).first );

        }

    }

   logDebug("TcpCommandFactories::getFactories() finished.");

   return hash;
}

TcpCommandFactories::~TcpCommandFactories() {

    logDebug("TcpCommandFactories::~TcpCommandFactories(): Destroy all Factories...");

    // Map von oben bis unten durchlaufen und alle Factories wieder freigeben...
    factoryGroupMap::iterator groupIT;
    for (groupIT = _factoryMap.begin(); groupIT != _factoryMap.end(); groupIT++) {

        logDebug("TcpCommandFactories::~TcpCommandFactories(): Access group %1", (*groupIT).first);

        factoryActionMap actionsMap = (*groupIT).second;

        // Nun die Aktion- Map durchlaufen und alle Factories löschen:
        factoryActionMap::iterator actionsIT;
        for (actionsIT = actionsMap.begin(); actionsIT != actionsMap.end(); actionsIT++) {

            logDebug("TcpCommandFactories::~TcpCommandFactories(): Destroy factory %1", (*actionsIT).first);

            // Factory zerstören:
            delete (*actionsIT).second;

            logDebug("TcpCommandFactories::~TcpCommandFactories(): Factory destroyed.");
        }

    }

    logDebug("TcpCommandFactories::~TcpCommandFactories(): finished!");
}
