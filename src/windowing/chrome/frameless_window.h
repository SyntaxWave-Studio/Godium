#ifndef FramelessWindow_H
#define FramelessWindow_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>

class FramelessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow() override = default;

    void setContentWidget(QWidget *widget);

    int resizeMargin() const;
    void setResizeMargin(int margin);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

    Qt::Edges hitZone(const QPoint &pos) const;

private:
    QVBoxLayout *layout;
    int m_resizeMargin = 12;
};

#endif