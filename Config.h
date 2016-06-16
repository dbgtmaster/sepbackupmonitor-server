#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

class QFile;

class Config
{
private:

    static Config* _instance;

    QHash<QString, QHash<QString, QString> > _confVars;
    QFile *_file;

    QSettings _config;

    Config();

public:
    static Config* instance();
    ~Config();

    QString get(const QString &group, const QString &key, const QString &standardValue = QString());
};

#endif // CONFIG_H
