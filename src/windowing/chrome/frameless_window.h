#ifndef FramelessWindow_H
#define FramelessWindow_H

#include "control_bar.h"

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QSplitter>

class FramelessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow() override = default;

    QWidget *contentWidget() const { return m_contentWidget; }
    void setContentWidget(QWidget *widget);

    int resizeMargin() const { return m_resizeMargin; }
    void setResizeMargin(int margin);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

    Qt::Edges hitZone(const QPoint &pos) const;

private:
    QWidget *m_central;
    QVBoxLayout *m_layout;

    QSplitter *m_barSplitter;
    ControlBar *m_controlBar;

    QWidget *m_contentWidget = nullptr;

    int m_resizeMargin = 12;
};

#endif