#include "LoggerConsoleAppender.h"

#include <iostream>

LoggerConsoleAppender::LoggerConsoleAppender(QObject *parent) :
    LoggerAbstractAppender(parent)
{
}

void LoggerConsoleAppender::write(const QString &msg) {

    std::cout << msg.toStdString().c_str() << std::endl;
}
