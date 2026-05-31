#include "frameless_window.h"

#include <QWindow>
#include <QMouseEvent>

FramelessWindow::FramelessWindow(QWidget *parent) : QMainWindow(parent)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
}

void FramelessWindow::setContentWidget(QWidget *widget)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        if (item->widget())
            item->widget()->setParent(nullptr);
        delete item;
    }

    if (widget)
        layout->addWidget(widget);
}

int FramelessWindow::resizeMargin() const
{
    return m_resizeMargin;
}

void FramelessWindow::setResizeMargin(int margin)
{
    m_resizeMargin = margin;
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && windowHandle())
    {
        Qt::Edges edges = hitZone(event->pos());

        if (edges != Qt::Edges(0))
        {
            windowHandle()->startSystemResize(edges);
        }
        else
        {
            windowHandle()->startSystemMove();
        }
    }
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    Qt::Edges edges = hitZone(event->pos());

    if (edges == (Qt::LeftEdge | Qt::TopEdge) || edges == (Qt::RightEdge | Qt::BottomEdge))
        setCursor(Qt::SizeFDiagCursor);
    else if (edges == (Qt::RightEdge | Qt::TopEdge) || edges == (Qt::LeftEdge | Qt::BottomEdge))
        setCursor(Qt::SizeBDiagCursor);
    else if (edges == Qt::TopEdge || edges == Qt::BottomEdge)
        setCursor(Qt::SizeVerCursor);
    else if (edges == Qt::LeftEdge || edges == Qt::RightEdge)
        setCursor(Qt::SizeHorCursor);
    else
        setCursor(Qt::ArrowCursor);
}

void FramelessWindow::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
}

Qt::Edges FramelessWindow::hitZone(const QPoint &pos) const
{
    Qt::Edges edges = Qt::Edges(0);

    if (pos.x() < m_resizeMargin)
        edges |= Qt::LeftEdge;
    if (pos.x() > width() - m_resizeMargin)
        edges |= Qt::RightEdge;
    if (pos.y() < m_resizeMargin)
        edges |= Qt::TopEdge;
    if (pos.y() > height() - m_resizeMargin)
        edges |= Qt::BottomEdge;

    return edges;
}