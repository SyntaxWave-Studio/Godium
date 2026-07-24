#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonObject>
#include <QString>

class Config : public QObject
{
    Q_OBJECT

public:
    virtual QString configName() const = 0;

protected:
    friend class ConfigManager;
    explicit Config(QObject *parent = nullptr);

    QString configFolder() const;
    QString configFilePath() const;

    QJsonObject &data() { return m_data; }
    const QJsonObject &data() const { return m_data; }

signals:
    void configChanged();

private:
    QJsonObject m_data;
};

#endif