#include "layout_window.h"

LayoutWindow::LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent) : FramelessWindow(parent)
{
    m_groupWidget = new QWidget();
    setContentWidget(m_groupWidget);

    m_groupSplitter = new VirtualSplitter(Qt::Horizontal);
    m_groupSplitter->setAllowDrop(true);
    m_groupSplitter->setAllowRemove(true);

    m_groupLayout = new QVBoxLayout(m_groupWidget);
    m_groupLayout->setContentsMargins(6, 6, 6, 6);
    m_groupLayout->setSpacing(0);
    m_groupLayout->addWidget(m_groupSplitter);

    connect(m_groupSplitter, &QObject::destroyed, this, [this]() { 
        window()->close(); 
    });

    if (virtualWindow)
    {
        VirtualGroup *virtualGroup = new VirtualGroup(virtualWindow);
        m_groupSplitter->addWidget(virtualGroup);
    }
}

void LayoutWindow::changeGroupParent(QWidget *parent)
{
    m_groupWidget->setParent(parent);
}
