#ifndef LOGGERABSTRACTAPPENDER_H
#define LOGGERABSTRACTAPPENDER_H

#include <QObject>

class LoggerAbstractAppender : public QObject
{
    Q_OBJECT
public:
    explicit LoggerAbstractAppender(QObject *parent = 0);

    // virutal destructor..
    virtual ~LoggerAbstractAppender();

    // Muss überladen werden, schreibt den String in die Datei / Konsole, oder wo auch immer...
    virtual void write(const QString &msg) = 0;
signals:

public slots:

};

#endif // LOGGERABSTRACTAPPENDER_H
