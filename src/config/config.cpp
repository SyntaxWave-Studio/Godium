#include "config.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QString>

Config::Config()
{
    m_configFile = QString("%1/%2/config.json").arg(QDir::currentPath()).arg(CONFIG_PATH);
}
    
bool Config::load()
{
    QFile file(m_configFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open config file for reading:" << m_configFile;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << error.errorString();
        return false;
    }
    
    m_data = doc.object();
    return true;
}

bool Config::save()
{
    QDir dir(QDir::cleanPath(m_configFile));
    dir.cdUp();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QFile file(m_configFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open config file for writing:" << m_configFile;
        return false;
    }
    
    QJsonDocument doc(m_data);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    return true;
}

QVariant Config::get(const QString& section, const QString& key, const QVariant& defaultValue) const
{
    if (!m_data.contains(section)) {
        return defaultValue;
    }
    
    QJsonObject sectionObj = m_data[section].toObject();
    if (!sectionObj.contains(key)) {
        return defaultValue;
    }
    
    QVariant value = sectionObj[key].toVariant();
    return value.isNull() ? defaultValue : value;
}

void Config::set(const QString& section, const QString& key, const QVariant& value)
{
    if (!m_data.contains(section)) {
        m_data[section] = QJsonObject();
    }
    
    QJsonObject sectionObj = m_data[section].toObject();
    sectionObj[key] = QJsonValue::fromVariant(value);
    m_data[section] = sectionObj;
}

