#ifndef VIRTUALWINDOW_H
#define VIRTUALWINDOW_H

#include <QWidget>
#include <QVariant>

class VirtualWindow : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualWindow(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~VirtualWindow() = default;
    virtual VirtualWindow *createNew() = 0;

    virtual QString tabTitle() const { return windowTitle(); }
    virtual void setTabTitle(const QString &title) { setWindowTitle(title); }

    virtual void initializeContent(const QVariant &data) = 0;
    virtual QString windowTitle() const { return m_title; }
    virtual void setWindowTitle(const QString &title) { m_title = title; }
    
signals:
    void tabTitleChanged(const QString &newTitle);

private:
    QString m_title;
};

#endif
