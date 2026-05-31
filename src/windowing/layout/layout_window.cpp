#include "layout_window.h"

LayoutWindow::LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent) : FramelessWindow(parent)
{
    m_groupWidget = new QWidget();
    setContentWidget(m_groupWidget);

    m_groupSplitter = new VirtualSplitter(Qt::Horizontal);
    m_groupSplitter->setAllowDrop(true);

    m_groupLayout = new QVBoxLayout(m_groupWidget);
    m_groupLayout->setContentsMargins(6, 6, 6, 6);
    m_groupLayout->setSpacing(0);
    m_groupLayout->addWidget(m_groupSplitter);

    if (virtualWindow)
    {
        VirtualGroup *virtualGroup = new VirtualGroup(virtualWindow);
        m_groupSplitter->addWidget(virtualGroup);
    }
}