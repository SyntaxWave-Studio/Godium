#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : LayoutWindow(nullptr, parent)
{

}

void MainWindow::setDockWindow(VirtualWindow *dock)
{
    if (m_dockPanel)
    {
        int idx = m_dockSplitter->indexOf(m_dockPanel);
        m_dockSplitter->replaceWidget(idx, nullptr);
        m_dockPanel = nullptr;
    }

    if (!dock)
        return;

    m_dockPanel = dock;
    m_dockPanel->setMinimumWidth(200);
    m_dockPanel->setMaximumWidth(400);

    m_dockSplitter->insertWidget(0, m_dockPanel);
    m_dockSplitter->setStretchFactor(0, 1);
    m_dockSplitter->setStretchFactor(1, 3);
}