#include "TcpCommands/customers/modCustomer.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"
#include "Config.h"

#include "Logger/Logger.h"

TcpCommand_customers_modCustomer::TcpCommand_customers_modCustomer() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_customers_modCustomer::exec() {

    quint32 customerId;
    _streamRead(customerId);

    QHash<QString, QVariant> columns;
    _streamRead(columns);

    // Liste der Spalten, welche bearbeitet werden dürfen:
    QStringList validCols;
    validCols << "backup_enabled" << "backup_prospective_jobs_mon"
              << "backup_prospective_jobs_tue" << "backup_prospective_jobs_wed" << "backup_prospective_jobs_thu"
              << "backup_prospective_jobs_fri" << "backup_prospective_jobs_sat" << "backup_prospective_jobs_sun"
              << "backup_group";

    // Wenn das erstellen von Kunden erlaut wurde:
    bool customerDataEditable = (Config::instance()->get("customers", "editable", "no") == "yes");
    if (customerDataEditable) {
        validCols << "customer_name" << "address" << "plz" << "city" << "main_email";

        if (customerId == 0) {
            if (!columns.contains("plz")) {
                columns["plz"] = "0";
            }
            if (!columns.contains("customer_name")) {
                return QString("Es wurde kein Kundenname definiert!");
            }
        }
    }
    else {
        _customerId = customerId;
    }

    /**
      * Eingaben überprüfen
      */
    if (columns.contains("customer_name")) {
        QString col = columns["customer_name"].toString();
        if (col.isEmpty()) {
            return QString("Es wurde kein Kundenname angegeben.");
        }
    }
    if (columns.contains("plz")) {
        if (columns["plz"].toString() != QString::number( columns["plz"].toInt() )) {
            qDebug() << columns["plz"].toString() << QString::number( columns["plz"].toInt() );
            return QString( ("Es wurde keine ungueltige PLZ angegeben.") );
        }
    }
    if (columns.contains("backup_prospective_jobs_mon")) {
        QVariant col = columns["backup_prospective_jobs_mon"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_mon: Ungueltiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Montag: Es wurde ein ungueltiger Wert angegeben.");
        }
    }
    if (columns.contains("backup_prospective_jobs_tue")) {
        QVariant col = columns["backup_prospective_jobs_tue"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_tue: Ungültiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Dienstag: Es wurde ein ungueltiger Wert angegeben.");
        }
    }
    if (columns.contains("backup_prospective_jobs_wed")) {
        QVariant col = columns["backup_prospective_jobs_wed"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_wed: Ungültiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Mittwoch: Es wurde ein ungueltiger Wert angegeben.");
        }
    }
    if (columns.contains("backup_prospective_jobs_thu")) {
        QVariant col = columns["backup_prospective_jobs_thu"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_thu: Ungültiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Donnerstag: Es wurde ein ungueltiger Wert angegeben.");
        }
    }
    if (columns.contains("backup_prospective_jobs_fri")) {
        QVariant col = columns["backup_prospective_jobs_fri"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_fri: Ungültiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Freitag: Es wurde ein ungueltiger Wert angegeben.");
        }
    }
    if (columns.contains("backup_prospective_jobs_sat")) {
        QVariant col = columns["backup_prospective_jobs_sat"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_sat: Ungültiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Samstag: Es wurde ein ungueltiger Wert angegeben.");
        }
    }
    if (columns.contains("backup_prospective_jobs_sun")) {
        QVariant col = columns["backup_prospective_jobs_sun"];
        if (col.type() != QVariant::Int) {
            return QString("backup_prospective_jobs_sun: Ungültiger Datentyp!");
        }
        if (col.toInt() < 0 || col.toInt() > 10000) {
            return QString("Anzahl der Jobs am Sonntag: Es wurde ein ungueltiger Wert angegeben.");
        }
    }

    if (columns.contains("backup_group") && columns["backup_group"].type() != QVariant::Int) {
        return QString("column backup_group must be a number!");
    }

    if (columns.count() <= 0) {
        return QString("No columns defined!");
    }

    QHash<QString, QVariant>::Iterator it;
    for (it = columns.begin(); it != columns.end(); it++) {
        QString col = it.key();
        if (!validCols.contains(col)) {
            logError("columns: Modify database column '%1' is not allowed! Bye!", col);
            closeSocket();
            return false;
        }
    }

    if (customerId != 0) {
        // Kunde wird bearbeitet:
        DatabaseQuery q = _db->update<QHash<QString, QVariant>::Iterator>(columns.begin(), columns.end(),
                                                                          "customers", "id", customerId);

        // true, wenn Spalten geändert wurden, ansonsten false...
        return (q.numAffectedRows() != 0);
    }
    else {
        // Kunde wird erstellt:
        DatabaseQuery q = _db->insert(columns, "customers", "id");
        q.next();
        _customerId = q.row("id").toInt();

        logNotice("Customer with id '%1' created");
        return true;
    }
}

void TcpCommand_customers_modCustomer::postExec(const QVariant &returnStatus) {

    _streamWrite(returnStatus);
    _streamWrite(_customerId);
}
