#include "TcpCommands/backupMonitor/getJobProtocol.h"

#include "Database/Database.h"
#include "Database/DatabaseQuery.h"

#include "Logger/Logger.h"

#include <QVector>
#include <QByteArray>

// Anzahl der Zeilen, welche pro Protokollseite angezeigt werden sollen.
#define LINES_PER_PROTOCOL_PAGE 500

TcpCommand_backupMonitor_getJobProtocol::TcpCommand_backupMonitor_getJobProtocol() : TcpAbstractCommand()
{
}

/**
  * Führt den eigentlichen Tcp- Befehl aus...
  * Lesen: _dataStreamRead >> var;
  * Schreiben: _dataStreamWrite << var;
  * Socket close: _readSocket->close();
  */
QVariant TcpCommand_backupMonitor_getJobProtocol::exec() {

    quint32 jobId, protocolType, protocolPage;
    _streamRead(jobId);
    _streamRead(protocolType);
    _streamRead(protocolPage);

    logNotice("Get protocol with id '%1'", QString::number(jobId));

    if (protocolPage < 1) protocolPage = 1;

    QString protocolTypeString;
    switch (protocolType) {
    case MAIN:
        protocolTypeString = "main";
        break;
    case PRE:
        protocolTypeString = "pre";
        break;
    case POST:
        protocolTypeString = "post";
        break;
    default:
        logWarning("Unknown protocol type!");
        closeSocket();
        return QVariant();
    }

    // Pfad zum Protokoll generieren:
    QString path = "/var/lib/sepbackupmonitor-server/backup-protocols/" + QString::number(jobId % 100) + "/" + QString::number(jobId % 99) + "/" + QString::number(jobId) + "_" + protocolTypeString;

    QFile protocolFile(path + "_" + QString::number(protocolPage));
    protocolFile.open(QIODevice::ReadOnly);
    QByteArray protocol;
    if (!protocolFile.isReadable()) {
        logNotice("Protocolfile %1 does not exists.", path);
    }
    else {

        // Damit wir php5 gzcompress kompatibel mit qUncompress() machen...
        protocol = protocolFile.readAll();
        quint32 uncompressedSize = protocol.size();
        protocol.prepend( ((uncompressedSize >> 0) & 0xFF) );
        protocol.prepend( ((uncompressedSize >> 8) & 0xFF) );
        protocol.prepend( ((uncompressedSize >> 16) & 0xFF) );
        protocol.prepend( ((uncompressedSize >> 24) & 0xFF) );
    }

    // Anzahl der Seiten:
    QFile protocolFilePages(path);
    protocolFilePages.open(QIODevice::ReadOnly);

    bool ok;
    quint32 totalPages = protocolFilePages.readAll().toInt(&ok);
    if (!ok) {
        logWarning("Could not convert the number of pages to integer! Break line or windows char at the end of line? Use 'echo -n 12345 > %1' to try!", path);
        totalPages = 99999999;
    }

    _streamWrite(protocol);
    _streamWrite(totalPages);

    return QVariant();
}

