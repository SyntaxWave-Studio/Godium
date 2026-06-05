#include "main_window.h"
#include "editor_window.h"

MainWindow::MainWindow(QWidget *parent) : LayoutWindow(nullptr, parent)
{
    m_dockSplitter = new VirtualSplitter(Qt::Horizontal);
    m_dockSplitter->setAllowRemove(false);

    groupLayout() -> replaceWidget(groupSplitter(), m_dockSplitter);
    m_dockSplitter->addWidget(groupSplitter());
    groupSplitter()->setAllowRemove(false);

    const QStringList files = {"main.cpp", "style.css", "hello.cpp"};
    for (const QString &fileName : files)
    {
        EditorWindow *editor = new EditorWindow();
        editor->initializeContent(fileName);

        VirtualGroup *group = new VirtualGroup(editor);
        groupSplitter()->addWidget(group);
    }

    EditorWindow *dock = new EditorWindow();
    dock->initializeContent("dock.cpp");
    setDockWindow(dock);
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