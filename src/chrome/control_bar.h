#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QFrame>

class ControlBar : public QFrame
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr, int height = 30);

    void addButton(QPushButton *button, int pos = 0);
    void removeButton(QPushButton *button);
    int buttonCount() const;

private:
    void setupUi();
    void setupStyle();

    void mousePressEvent(QMouseEvent *event) override;

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