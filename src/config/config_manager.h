#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "config.h"

#include <QMap>
#include <QString>
#include <QObject>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    static ConfigManager& instance();

    template <typename TConfig>
    TConfig &getConfig();

private:
    explicit ConfigManager(QObject* parent = nullptr);

    QMap<QString, Config*> m_configs;
};

#endif