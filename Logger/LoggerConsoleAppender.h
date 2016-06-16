#ifndef LOGGERCONSOLEAPPENDER_H
#define LOGGERCONSOLEAPPENDER_H

#include "LoggerAbstractAppender.h"

class LoggerConsoleAppender : public LoggerAbstractAppender
{
    Q_OBJECT
public:
    explicit LoggerConsoleAppender(QObject *parent = 0);

    void write(const QString &msg);

};

#endif // LOGGERCONSOLEAPPENDER_H
