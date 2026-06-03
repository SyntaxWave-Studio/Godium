#include "frameless_window.h"

#include <QWindow>
#include <QMouseEvent>

FramelessWindow::FramelessWindow(QWidget *parent) : QMainWindow(parent)
{
    setStyleSheet("QMainWindow { background: #0b0b0c; }");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

    m_central = new QWidget(this);
    setCentralWidget(m_central);

    m_layout = new QVBoxLayout(m_central);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_controlBar = new ControlBar();
    m_layout->addWidget(m_controlBar);
}

void FramelessWindow::setContentWidget(QWidget *widget)
{
    if (m_contentWidget)
    {
        m_layout->removeWidget(m_contentWidget);
        m_contentWidget->setParent(nullptr);
        m_contentWidget->deleteLater();
    }

    m_contentWidget = widget;

    if (m_contentWidget)
    {
        m_contentWidget->setParent(m_central);
        m_layout->addWidget(m_contentWidget);
        m_layout->setStretch(1, 1);
    }
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

        if (window()->isMaximized() || edges == Qt::Edges(0))
            windowHandle()->startSystemMove();

        if (!window()->isMaximized() && edges != Qt::Edges(0))
            windowHandle()->startSystemResize(edges);
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