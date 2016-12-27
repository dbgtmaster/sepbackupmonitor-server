#include "Config.h"

#include <QSettings>
#include <QDebug>
#include <QStringList>
#include <QFile>

#include <iostream>

Config* Config::_instance = 0;

Config::Config() : _config(QSettings::IniFormat, QSettings::UserScope, "backupmonitor", "server")
{

    _file = new QFile("/usr/local/sepbackupmonitor/etc/server.ini");
    _file->open(QIODevice::ReadOnly);
    if (!_file->isReadable()) {
        std::cout << "/usr/local/etc/sepbackupmonitor/server.ini is not readable!";
        exit(1);
    }

    /**
      * Nun parsen wir das Config File:
      */
    QTextStream stream(_file);
    QHash<QString, QHash<QString, QString> >::Iterator it;
    bool groupFound = false;
    while(!stream.atEnd()) {
        QString line = stream.readLine().trimmed();


        if (line.startsWith("#")) continue;
        if (line.count() == 0) continue;

        if (line.startsWith("[") && line.endsWith("]")) {
            QString groupName = line.mid(1, line.count() - 2);
            if (_confVars.contains(groupName)) {
                it = _confVars.find(groupName);
            }
            else {
                it = _confVars.insert( line.mid(1, line.count() - 2), QHash<QString, QString>() );
            }

            groupFound = true;
            continue;
        }

        if (!groupFound) {
            std::cout << "config must start with a [group]!";
            exit(1);
        }

        QString key = line.mid(0, (line.indexOf("="))).trimmed();
        QString value = line.mid((line.indexOf("=") + 1)).trimmed();

        (*it).insert(key, value);
    }
}

Config::~Config() {
    delete _file;
}

Config* Config::instance() {

    if (_instance == 0) _instance = new Config;
    return _instance;
}

QString Config::get(const QString &group, const QString &key, const QString &standardValue) {

    QHash<QString, QString> vars = _confVars.value(group);
    if (vars.contains(key)) {
        return vars.value(key);
    }
    else {
        return standardValue;
    }
}
