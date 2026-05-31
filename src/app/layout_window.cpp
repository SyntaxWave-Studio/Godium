#include "layout_window.h"

LayoutWindow::LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent) : FramelessWindow(parent)
{
    setStyleSheet("QMainWindow { background: #0b0b0c; }");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

    m_menuSplitter = new QSplitter(Qt::Vertical);
    m_menuSplitter->setHandleWidth(0);
    m_menuSplitter->setStretchFactor(0, 0);
    m_menuSplitter->setStretchFactor(1, 1);

    m_controlBar = new ControlBar(m_menuSplitter);
    m_menuSplitter->addWidget(m_controlBar);

    m_contentWidget = new QWidget(m_menuSplitter);
    m_menuSplitter->addWidget(m_contentWidget);

    m_layout = new QVBoxLayout(m_contentWidget);
    m_layout->setContentsMargins(6, 6, 6, 6);
    m_layout->setSpacing(0);

    m_groupSplitter = new VirtualSplitter(Qt::Horizontal, m_contentWidget);
    m_layout->addWidget(m_groupSplitter);

    if (virtualWindow)
    {
        VirtualGroup *virtualGroup = new VirtualGroup(virtualWindow);
        m_groupSplitter->addWidget(virtualGroup);
    }

    setContentWidget(m_menuSplitter);
}