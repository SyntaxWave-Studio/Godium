#include "layout_window.h"

LayoutWindow::LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent) : ControlBarWindow(parent)
{
    m_groupSplitter = new VirtualSplitter(Qt::Horizontal);
    setContentWidget(m_groupSplitter);

    if (virtualWindow)
    {
        VirtualGroup *virtualGroup = new VirtualGroup(virtualWindow);
        m_groupSplitter->addWidget(virtualGroup);
    }
}