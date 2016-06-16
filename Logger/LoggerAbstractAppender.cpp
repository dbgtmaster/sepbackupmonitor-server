#include "LoggerAbstractAppender.h"

#include <iostream>

LoggerAbstractAppender::LoggerAbstractAppender(QObject *parent) :
    QObject(parent)
{

}

LoggerAbstractAppender::~LoggerAbstractAppender() {

}

void LoggerAbstractAppender::write(const QString &msg) {

    std::cout << msg.toStdString() << std::endl;
}
