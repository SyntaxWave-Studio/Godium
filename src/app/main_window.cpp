#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : LayoutWindow(nullptr, parent)
{
    m_dockSplitter = new QSplitter();
    m_dockSplitter->setHandleWidth(6);

    groupLayout()->replaceWidget(groupSplitter(), m_dockSplitter);
    m_dockSplitter->addWidget(groupSplitter());
}

void MainWindow::setDockWindow(VirtualWindow *dock)
{
    if (m_dockPanel)
    {
        int idx = m_dockSplitter->indexOf(m_dockPanel);
        m_dockSplitter->replaceWidget(idx, nullptr);
        m_dockPanel = nullptr;
    }

    m_dockPanel = dock;
    if (!m_dockPanel)
        return;

    m_dockSplitter->insertWidget(0, m_dockPanel);
    m_dockSplitter->setStretchFactor(0, 1);
    m_dockSplitter->setStretchFactor(1, 2);
}