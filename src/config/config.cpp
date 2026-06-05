#include "config.h"
#include "config_manager.h"

Config::Config(QObject *parent) : QObject(parent) { }

QString Config::configFolder() const
{
    return CONFIG_PATH;
}

QString Config::configFilePath() const
{
    return QString("%1/%2/%3.json")
        .arg(QDir::currentPath())
        .arg(configFolder())
        .arg(configName());
}