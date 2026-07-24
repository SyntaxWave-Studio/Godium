#include "config.h"
#include "config_manager.h"

#include <QCoreApplication>
#include <QDir>

Config::Config(QObject *parent) : QObject(parent) { }

QString Config::configFolder() const
{
    QString baseDir = QCoreApplication::applicationDirPath();
    QString relativePath = (CONFIG_PATH && strlen(CONFIG_PATH) > 0) ? QString(CONFIG_PATH) : QString("config");
    return QDir(baseDir).filePath(relativePath);
}

QString Config::configFilePath() const
{
    return QString("%1/%2.json")
        .arg(configFolder())
        .arg(configName());
}