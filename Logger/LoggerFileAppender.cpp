#include "LoggerFileAppender.h"

LoggerFileAppender::LoggerFileAppender(const QString &fileName, QObject *parent) :
    LoggerAbstractAppender(parent)
{
    _fileHandler.setFileName(fileName);

    if (!_fileHandler.open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
        // Fehler beim öffnen der Datei :-( ...
        std::cout << "LoggerFileAppender: Log-File " << fileName.toStdString().c_str() << " konnte nicht geoeffnet werden :(" << std::endl;
    }
}

LoggerFileAppender::~LoggerFileAppender() {

    _fileHandler.close();
}

void LoggerFileAppender::write(const QString &msg) {

    _fileHandler.write(msg.toLatin1());
    _fileHandler.write("\n");
    _fileHandler.flush();
}
