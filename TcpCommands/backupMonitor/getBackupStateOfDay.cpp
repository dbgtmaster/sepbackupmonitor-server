#include "TcpCommands/backupMonitor/getBackupStateOfDay.h"

#include "Utils/UtilBackupMonitor.h"

#include "Logger/Logger.h"

#include "QHash"
#include "QList"

TcpCommand_backupMonitor_getBackupStateOfDay::TcpCommand_backupMonitor_getBackupStateOfDay() : TcpAbstractCommand()
{
}

/**
  * Fhrt den eigentlichen Tcp- Befehl aus...
  * Lesen: _streamRead(var)
  * Schreiben: _streamWrite(var);
  * Lesen & Schreiben werfen eine Exception, welche vom TcpThread aufgefangen wird, wenn ungltige Daten gelesen / geschrieben werden...
  * Auf Datastream direkt zugreifen (zB. um zu berprfen ob noch Daten vorhanden): _dataStreamRead, _dataStreamWrite
  * Socket close: _readSocket->close();
  *
  * @return QVariant: Es kann ein beliebiger Wert zurckgegeben werden... Wenn void postExec(const QVariant &var)  berladen wird,
  *						kann auf diesen Wert wieder zugegriffen werden.
  */
QVariant TcpCommand_backupMonitor_getBackupStateOfDay::exec() {
    
    QList<QDate> dates;
    _streamRead(dates);

    quint32 customerId;
    _streamRead(customerId);

    QList< QHash<QByteArray, QVariant> > list;
    QList<QDate>::Iterator it;
    for (it = dates.begin(); it != dates.end(); it++) {

        logDebug("Start getting next date...");

        QDate date = (*it);

        // Is date valid?
        if (!date.isValid()) {
            logWarning("Invalid date given, object is not valid! Bye");
            closeSocket();
            return true;
        }

        UtilBackupMonitor::stateOfBackupDay::backupState state = UtilBackupMonitor::stateOfBackupDay::get(customerId, date);

        QHash<QByteArray, QVariant > data;
        data["date"] = date;
        data["total"] = state.total;
        data["totalWithoutMainJobs"] = state.totalWithoutMainJobs;
        data["successful"] = state.successful;
        data["warning"] = state.warning;
        data["error"] = state.error;
        data["aborted"] = state.aborted;
        data["running"] = state.running;

        list.append(data);

        // Wir liefern maximal 100 Backup Tage zurück...
        if (list.size() > 100) {
            break;
        }
    }

    _streamWrite(list);

    return true;
}


