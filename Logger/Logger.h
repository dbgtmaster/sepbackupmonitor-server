#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

#include <QString>
#include <QList>

#include <QMutex>

#include "LoggerAbstractAppender.h"

class Logger : public QObject {
    Q_OBJECT
public:
    enum Categories {
        DEBUG = 4,
        NOTICE = 3,
        WARNING = 2,
        ERROR = 1,
        FATAL = 0
    };

private:

    static Logger* _logger;

    Categories _logLevel;

    // Eine Liste mit allen verfügbaren appenders...
    QList<LoggerAbstractAppender*> _appenders;

    // Konstruktor ist private für singleton objekt...
    Logger(QObject *parent = 0);

    // Bereitet die Nachricht vor, bevor diese an die Appenders übermittelt wird..
    // Fügt Datum & co dran...
    QString prepareMsg(const QString &logLevel, const QString &msg);

    // Übermittelt die nachricht an alle Appender...
    void writeAppenders(const QString &msg);

    QMutex _mutexWriter;

public:

    ~Logger();

    // Gibt das Singleton- Objekt zurück...
    static Logger* instance();

    // Setzt die Appender (die Klasse, welche die Log- Einträge ins Ziel schreibt (console, file,..)
    void addAppender(LoggerAbstractAppender *app);

    // Die Methoden, um Log- Einträge je nach Stufe zu schreiben...
    void debug(const QString &msg);
    void notice(const QString &msg);
    void warning(const QString &msg);
    void error(const QString &msg);
    void fatal(const QString &msg);

    inline void setLogLevel(Categories l) {
        _logLevel = l;
    }
};

// Hilfsfunktionen, um nicht immer "Logger::instace()->debug(msg)" schreiben zu müssen...
inline void logDebug(const QString &msg) {
    Logger::instance()->debug(msg);
}
inline void logDebug(const QString &msg, const QString &arg1) {
    Logger::instance()->debug( QString(msg).arg(arg1) );
}
inline void logDebug(const QString &msg, const QString &arg1, const QString &arg2) {
    Logger::instance()->debug( QString(msg).arg(arg1, arg2) );
}

inline void logNotice(const QString &msg) {
    Logger::instance()->notice( QString(msg) );
}
inline void logNotice(const QString &msg, const QString &arg1) {
    Logger::instance()->notice( QString(msg).arg(arg1) );
}
inline void logNotice(const QString &msg, const QString &arg1, const QString &arg2) {
    Logger::instance()->notice( QString(msg).arg(arg1, arg2) );
}
inline void logNotice(const QString &msg, const QString &arg1, const QString &arg2, const QString &arg3) {
    Logger::instance()->notice( QString(msg).arg(arg1, arg2, arg3) );
}

inline void logWarning(const QString &msg) {
    Logger::instance()->warning(msg);
}

inline void logWarning(const QString &msg, const QString &arg1) {
    Logger::instance()->warning( QString(msg).arg(arg1) );
}

inline void logWarning(const QString &msg, const QString &arg1, const QString &arg2) {
    Logger::instance()->warning( QString(msg).arg(arg1, arg2) );
}

inline void logError(const QString &msg) {
    Logger::instance()->error(msg);
}
inline void logError(const QString &msg, const QString &arg1) {
    Logger::instance()->error( QString(msg).arg(arg1) );
}
inline void logError(const QString &msg, const QString &arg1, const QString &arg2) {
   Logger::instance()->error( QString(msg).arg(arg1, arg2) );
}

inline void logFatal(const QString &msg) {
    Logger::instance()->fatal(msg);
}
inline void logFatal(const QString &msg, const QString &arg1) {
    Logger::instance()->fatal( QString(msg).arg(arg1) );
}
inline void logFatal(const QString &msg, const QString &arg1, const QString &arg2) {
    Logger::instance()->fatal( QString(msg).arg(arg1, arg2) );
}
inline void logFatal(const QString &msg, const QString &arg1, const QString &arg2, const QString &arg3) {
    Logger::instance()->fatal( QString(msg).arg(arg1, arg2, arg3) );
}

#endif // LOGGER_H
