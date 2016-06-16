#ifndef LOGGERFILEAPPENDER_H
#define LOGGERFILEAPPENDER_H

#include "LoggerAbstractAppender.h"

#include <iostream>

#include <QFile>

class LoggerFileAppender : public LoggerAbstractAppender
{
private:
    QFile _fileHandler;

public:
    explicit LoggerFileAppender(const QString &fileName, QObject* parent = 0);
    ~LoggerFileAppender();

    void write(const QString &msg);
};

#endif // LOGGERFILEAPPENDER_H
