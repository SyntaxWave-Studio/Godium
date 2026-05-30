#include "layout_window.h"

LayoutWindow::LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent) : QMainWindow(parent)
{
    setStyleSheet("QMainWindow { background: #0b0b0c; }");
    setAttribute(Qt::WA_DeleteOnClose);

    m_central = new QWidget(this);
    setCentralWidget(m_central);

    m_layout = new QVBoxLayout(m_central);
    m_layout->setContentsMargins(6, 6, 6, 6);
    m_layout->setSpacing(0);

    m_groupSplitter = new VirtualSplitter(Qt::Horizontal, m_central);
    m_layout->addWidget(m_groupSplitter);

    connect(m_groupSplitter, &QObject::destroyed, this, [this]() {
        close(); 
    });

    if (virtualWindow)
    {
        VirtualGroup *virtualGroup = new VirtualGroup(virtualWindow);
        m_groupSplitter->addWidget(virtualGroup);
    }
}
