#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QJsonObject>
#include <QVariant>
#include <QDir>

#ifndef CONFIG_PATH
#define CONFIG_PATH "config"
#endif

class Config
{
public:
    virtual ~Config() = default;
    
    bool load();
    bool save();
    
protected:
    Config();
    
    QJsonObject& data() { return m_data; }
    const QJsonObject& data() const { return m_data; }
    QString& configFile() { return m_configFile; }
    
    QVariant get(const QString& section, const QString& key, const QVariant& defaultValue = QVariant()) const;
    void set(const QString& section, const QString& key, const QVariant& value);
    
private:
    QJsonObject m_data;
    QString m_configFile;
};

#endif
