#include "main_window.h"
#include "editor_window.h"
#include "virtual_group.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("GodiumIDE");
    a.setApplicationDisplayName("GodiumIDE");
    a.setOrganizationName("SyntaxWaveStudio");

    a.setQuitOnLastWindowClosed(true);
    a.setStyle("Fusion");

    MainWindow *w = new MainWindow();

    const QStringList files = {"main.cpp", "style.css"};
    for (const QString &fileName : files)
    {
        EditorWindow *editor = new EditorWindow();
        editor->initializeContent(fileName);

        VirtualGroup *group = new VirtualGroup();
        group->addWindow(editor, editor->windowTitle());

        w->splitter()->addWidget(group);
    }

    w->showNormal();
    return a.exec();
}
