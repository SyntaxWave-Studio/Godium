#include "windowing_config.h"
#include <QString>

WindowingConfig& WindowingConfig::instance()
{
    static WindowingConfig config;
    return config;
}

WindowingConfig::WindowingConfig()
{
    configFile() = QString("%1/%2/windowing.json").arg(QDir::currentPath()).arg(CONFIG_PATH);
    
    QJsonObject windowing;
    windowing["spacing"] = 4;
    windowing["rounded_corners"] = true;
    data() = windowing;
}

bool WindowingConfig::spacing() const
{
    return data().contains("spacing") ? data()["spacing"].toBool() : 4;
}

bool WindowingConfig::roundedCorners() const
{
    return data().contains("rounded_corners") ? data()["rounded_corners"].toBool() : true;
}

void WindowingConfig::setSpacing(bool value)
{
    data()["spacing"] = value;
}

void WindowingConfig::setRoundedCorners(bool value)
{
    data()["rounded_corners"] = value;
}

