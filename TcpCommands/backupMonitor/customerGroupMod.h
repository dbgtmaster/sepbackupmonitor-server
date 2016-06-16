/**
  * Modifiziert eine Gruppe
  *
  * @incoming-stream:
  *     @ ::ACTION              Ob Gruppe bearbeitet oder erstellt wird.
  *     @ quint32 id            ID der zu bearbeitenden Gruppe (beim erstellen wird ID ignoriert)
  *     @ QHash<QString, QVariant> cols     Spalten, welche geändert werden sollen...
  *                                     position: Darf den Wert 0 -> UP, 1 -> DOWN haben.
  *
  * @outcoming-stream:
  *     @ bool success          true, wenn Aktion erfolgreich war, ansonsten false
  *     @ QString error         Fehlermeldung, falls Aktion nicht erfolgreich war.
  */

#ifndef TCPCOMMAND_BACKUPMONITOR_CUSTOMERGROUPMOD_H
#define TCPCOMMAND_BACKUPMONITOR_CUSTOMERGROUPMOD_H

#include <TcpAbstractCommand.h>

class TcpCommand_backupMonitor_customerGroupMod : public TcpAbstractCommand
{
private:
    enum ACTION {
        CREATE,
        MODIFY
    };

    enum POSITION {
        UP, DOWN
    };

public:
    
    TcpCommand_backupMonitor_customerGroupMod();
    
    QVariant exec();
    
    void postExec(const QVariant &var);
    
};

#endif // TCPCOMMAND_BACKUPMONITOR_CUSTOMERGROUPMOD_H



