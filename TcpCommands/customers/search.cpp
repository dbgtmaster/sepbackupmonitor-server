#include "TcpCommands/customers/search.h"
#include "Logger/Logger.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include <QList>

TcpCommand_customers_search::TcpCommand_customers_search() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_customers_search::exec() {

    // Durchzusuchender Filter
    QStringList filter;
    _streamRead(filter);

    // Welche Spalten durchsucht werden sollen:
    QStringList filterCols;
    _streamRead(filterCols);

    // Welche Spalten sollen zurückgegeben werden...
    QStringList returnCols;
    _streamRead(returnCols);

    returnCols.append("id");

    // Gültige Spalten:
    QStringList validCols;
    validCols << "id" << "customer_name" << "address" << "plz" << "city" << "backup_enabled"
              << "backup_prospective_jobs_mon"
              << "backup_prospective_jobs_tue" << "backup_prospective_jobs_wed" << "backup_prospective_jobs_thu"
              << "backup_prospective_jobs_fri" << "backup_prospective_jobs_sat" << "backup_prospective_jobs_sun"
              << "backup_last_hello" << "backup_sync_client_version" << "backup_group" << "date_last_log_entry"
              << "username_of_last_log_entry";

    if (filterCols.count() == 0) {
        logError("No filterCols set! Bye");
        closeSocket();
        return false;
    }

    if (returnCols.count() == 0) {
        logError("No returnColumns set! Bye");
        closeSocket();
        return false;
    }

    // Würden gültige Spalten angegeben?
    foreach(QString col, filterCols) {
        if (!validCols.contains(col)) {
            logError("filterCols: Column '%1' is not allowed! Bye!", col);
            closeSocket();
            return false;
        }
    }

    // Würden gültige Spalten angegeben?
    foreach(QString col, returnCols) {
        if (!validCols.contains(col)) {
            logError("returnCols: Database return column '%1' is not allowed! Bye!", col);
            closeSocket();
            return false;
        }
    }

    // Get the date of the last log entry?
    bool getDateLastLogEntry = false;
    if (returnCols.contains("date_last_log_entry") || returnCols.contains("username_of_last_log_entry")) {
        getDateLastLogEntry = true;
        returnCols.removeAll("date_last_log_entry");
        returnCols.removeAll("username_of_last_log_entry");
    }

    // Datenbank SQL zusammenbauen:
    QString sql = "SELECT " + returnCols.join(", ") + " FROM customers WHERE ";

    // Bedingungen der SQL:
    QStringList::Iterator i;
    QStringList::Iterator iSearch;

    for (i = filterCols.begin(); i != filterCols.end(); i++) {

        if (i != filterCols.begin()) sql = sql + " OR ";
        sql = sql + "(";

        for (iSearch = filter.begin(); iSearch != filter.end(); iSearch++) {

            QString filterEscape = _db->escape( "%" + *iSearch + "%" );

            if (iSearch != filter.begin()) sql = sql + " OR ";

            if (*i == "plz" || *i == "id") {
                sql = sql + " " + (*i) + " = " + QString::number( (*iSearch).toInt() ) + " ";
            }
            else if (*i == "backup_enabled") {
                if ((*iSearch).toInt() == 1) {
                    sql = sql + " " + (*i) + " = true ";
                }
                else {
                    sql = sql + " " + (*i) + " = false ";
                }
            }
            else {
                sql = sql + " LOWER(" + (*i) + ") LIKE " + filterEscape.toLower() + " ";
            }
        }

        sql = sql + ")";
    }

    sql = sql + " ORDER BY customer_name";
    QList< QHash<QString, QVariant> > returnList;
    DatabaseQuery q = _db->query(sql);
    while (q.next()) {

        hashColumns columns = q.getAllColumns();

        // Get last log entry for this customer
        if (getDateLastLogEntry) {
            DatabaseQuery result = _db->query("SELECT u.username, l.date FROM backup_jobs_log AS l "
                                              "LEFT JOIN users u on u.id = l.userid"
                                              " WHERE l.customerid = " + columns["id"].toString() + " ORDER BY l.date DESC LIMIT 1");
            result.next();
            columns.insert("date_last_log_entry", result.row("date"));
            columns.insert("username_of_last_log_entry", result.row("username"));
        }

        returnList.append(columns);
    }

    _streamWrite(returnList);

    return true;
}

