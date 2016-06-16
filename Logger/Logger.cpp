#include "Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

Logger* Logger::_logger = 0;

// Konstruktor, ist private...
Logger::Logger(QObject *parent) : QObject(parent) {

}

Logger::~Logger() {

    if (_logger != 0) {
        delete _logger;
    }
}

// Erstellt das Singleton Objekt:
Logger* Logger::instance() {

    if (_logger == 0) {
        _logger = new Logger();
    }

    return _logger;
}

// Fügt einen Appender hinzu...
void Logger::addAppender(LoggerAbstractAppender *app) {
    _appenders.append(app);
}

void Logger::writeAppenders(const QString &msg) {

    _mutexWriter.lock();

    for (int i = 0; i < _appenders.count(); i++) {
        _appenders.at(i)->write(msg);
    }

    _mutexWriter.unlock();
}

QString Logger::prepareMsg(const QString &logLevel, const QString &msg) {

    return QString("%1 [%2] [%3]: %4")
            .arg(QDateTime::currentDateTime().toString(),
                 logLevel,
                 QString::number(QThread::currentThreadId()),
                 msg
                );
}

void Logger::debug(const QString &msg) {

    if (_logLevel < DEBUG) return;
    writeAppenders( prepareMsg("DEBUG", msg) );
}

void Logger::notice(const QString &msg) {

    if (_logLevel < NOTICE) return;
    writeAppenders( prepareMsg("NOTICE", msg) );
}

void Logger::warning(const QString &msg) {

    if (_logLevel < WARNING) return;
    writeAppenders( prepareMsg("WARNING", msg) );
}

void Logger::error(const QString &msg) {

    if (_logLevel < ERROR) return;
    writeAppenders( prepareMsg("ERROR", msg) );
}

void Logger::fatal(const QString &msg) {

    if (_logLevel < FATAL) return;
    writeAppenders( prepareMsg("FATAL", msg) );
}
