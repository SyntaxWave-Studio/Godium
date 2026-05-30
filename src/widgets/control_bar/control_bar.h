#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QFrame>

class ControlBar : public QFrame
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr);

    void addButton(QPushButton *button, int pos = 0);
    void removeButton(QPushButton *button);
    int buttonCount() const;

private:
    void setupStyle();

    QPushButton *createControlButton(const QString &icon, const QString &tooltip);

    QHBoxLayout *m_layout;
    QWidget *m_leftContainer;
    QHBoxLayout *m_leftLayout;
    QWidget *m_rightContainer;
    QHBoxLayout *m_rightLayout;

    QList<QPushButton *> m_leftButtons;
    QPushButton *m_minimizeBtn;
    QPushButton *m_maximizeBtn;
    QPushButton *m_closeBtn;
};

#endif