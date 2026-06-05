#include "config_manager.h"

#include <QDebug>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {}

ConfigManager &ConfigManager::instance()
{
    static ConfigManager manager;
    return manager;
}

template <typename TConfig>
TConfig &ConfigManager::getConfig()
{
    QString name = TConfig::staticMetaObject.className();

    if (!m_configs.contains(name))
    {
        TConfig *cfg = new TConfig(this);
        cfg->load();
        m_configs[name] = cfg;
    }

    return static_cast<TConfig &>(*m_configs[name]);
}