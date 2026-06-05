#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <QDir>

class Config : public QObject
{
    Q_OBJECT

public:
    virtual QString configName() const = 0;

protected:
    QString configFolder() const { return CONFIG_PATH; }
    QString configFilePath() const 
    {
        return QString("%1/%2/%3.json")
            .arg(QDir::currentPath())
            .arg(configFolder())
            .arg(configName());
    }
};

#endif