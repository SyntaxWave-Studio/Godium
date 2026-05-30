#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : LayoutWindow(nullptr, parent)
{
    m_menuBar = new ControlBar(m_central);
    m_menuSplitter = new VirtualSplitter(Qt::Vertical, m_central);
    m_bodySplitter = new VirtualSplitter(Qt::Horizontal, m_central);

    m_menuSplitter->setAllowRemove(false);
    m_bodySplitter->setAllowRemove(false);
    m_groupSplitter->setAllowRemove(false);

    m_menuSplitter->setAllowDrop(false);
    m_bodySplitter->setAllowDrop(false);

    m_menuSplitter->addWidget(m_menuBar);
    m_menuSplitter->addWidget(m_bodySplitter);
    m_bodySplitter->addWidget(m_groupSplitter);

    m_layout->removeWidget(m_groupSplitter);
    m_layout->addWidget(m_menuSplitter);
}

void MainWindow::setDockWindow(VirtualWindow *dock)
{
    if (m_dockPanel)
    {
        int idx = m_bodySplitter->indexOf(m_dockPanel);
        m_bodySplitter->replaceWidget(idx, nullptr);
        m_dockPanel = nullptr;
    }

    if (!dock)
        return;

    m_dockPanel = dock;
    m_dockPanel->setMinimumWidth(200);
    m_dockPanel->setMaximumWidth(400);

    m_bodySplitter->insertWidget(0, m_dockPanel);
    m_bodySplitter->setStretchFactor(0, 1);
    m_bodySplitter->setStretchFactor(1, 3);
}