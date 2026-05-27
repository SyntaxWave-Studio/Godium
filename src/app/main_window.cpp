#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : LayoutWindow(nullptr, parent)
{
    m_mainSplitter = new VirtualSplitter(Qt::Horizontal, m_central);
    m_mainSplitter->setAllowDrop(false);
    m_mainSplitter->addWidget(m_groupSplitter);

    m_layout->removeWidget(m_groupSplitter);
    m_layout->addWidget(m_mainSplitter);

    m_mainSplitter->setAllowRemove(false);
    m_groupSplitter->setAllowRemove(false);
}

void MainWindow::setDockWindow(VirtualWindow *dock)
{
    if (m_dockPanel)
    {
        int idx = m_mainSplitter->indexOf(m_dockPanel);
        m_mainSplitter->replaceWidget(idx, nullptr);
        m_dockPanel = nullptr;
    }

    if (!dock)
        return;

    m_dockPanel = dock;
    m_dockPanel->setMinimumWidth(200);
    m_dockPanel->setMaximumWidth(400);

    m_mainSplitter->insertWidget(0, m_dockPanel);
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 3);
}