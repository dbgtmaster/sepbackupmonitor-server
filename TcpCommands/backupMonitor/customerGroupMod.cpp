#include "TcpCommands/backupMonitor/customerGroupMod.h"

#include "Logger/Logger.h"

TcpCommand_backupMonitor_customerGroupMod::TcpCommand_backupMonitor_customerGroupMod() : TcpAbstractCommand()
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
QVariant TcpCommand_backupMonitor_customerGroupMod::exec() {
    
    ACTION action;
    quint32 action_, id;
    QHash<QString, QVariant> cols;
    _streamRead(action_);
    _streamRead(id);
    _streamRead(cols);
    action = static_cast<ACTION>(action_);

    if (action != CREATE && action != MODIFY) {
        closeSocket();
        return false;
    }

    DB db = DatabasePool::get();

    if (action == MODIFY) {

        quint32 count = db->query("SELECT COUNT(*) AS count FROM backup_customer_groups WHERE id = " + QString::number(id)).fetchCol("count").toInt();
        if (count != 1) {
            return "Invalid ID given.";
        }
    }

    mapColumns dbCols;

    if (cols.contains("name")) {
        QString name = cols["name"].toString();

        if (name.isEmpty()) {
            return "Es wurde kein Name angegeben.";
        }
        if (name.size() > 50) {
            return "Name darf maximal 50 Zeichen lang sein.";
        }

        dbCols["name"] = name;
    }
    else if (action == CREATE) {
        return "Es wurde kein Name angegeben.";
    }

    if (cols.contains("description")) {
        dbCols["description"] = cols["description"].toString();
    }
    if (cols.contains("auto_expand")) {
        dbCols["auto_expand"] = cols["auto_expand"].toBool();
    }

    // Position kann nur beim Update bestimmt werden:
    if (action == CREATE) {
        dbCols["position"] = QStringList() << "(SELECT COUNT(*) FROM backup_customer_groups) + 1";
    }
    else {
        // Position Update wird direkt beim absetzen der SQL geregelt..
    }

    if (action == CREATE) {

        DatabasePool::get()->insert(dbCols, "backup_customer_groups");
    }
    else {

        DB db = DatabasePool::get();

        db->transaction(); {

            // Position auf / abbewegen...
            if (cols.contains("position")) {

                // Aktuelle Position:
                quint32 position = db->query("SELECT position FROM backup_customer_groups WHERE id = " + QString::number(id)).fetchCol("position").toInt();

                if (position == 0) {
                    db->rollback();
                    return "There is a problem with the internal position id. position number is 0, but it must greater then 0! Workaround: Please create and remove the created group, to start an internal resort of the position ids!";
                }

                if (static_cast<POSITION>(cols["position"].toInt()) == UP) {
                    dbCols["position"] = position - 1;
                }
                else {
                    dbCols["position"] = position + 1;
                }

                DatabaseQuery r = db->query( QString("UPDATE backup_customer_groups SET position = "+ QString::number(position) +" WHERE position = " + dbCols["position"].toString() ) );
                if (r.numAffectedRows() != 1) {
                    db->rollback();
                    return "Moving failed. First group moved up / Last group moved down?";
                }
            }
            if (!dbCols.isEmpty()) {
                db->update(dbCols, "backup_customer_groups", "id", id);
            }

        } db->commit();
    }

    return true;
}

void TcpCommand_backupMonitor_customerGroupMod::postExec(const QVariant &var) {

    if (var.type() == QVariant::Bool) {
        _streamWrite(true);
        _streamWrite(QString());
    }
    else {
        _streamWrite(false);
        _streamWrite(var.toString());
    }
}
