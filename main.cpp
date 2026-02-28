#include <QApplication>
#include "MainWindow.h"
#include "EditorWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("GodiumIDE");
    a.setOrganizationName("SyntaxWaveStudio");
    a.setApplicationDisplayName("GodiumIDE");

    a.setQuitOnLastWindowClosed(true);
    a.setStyle("Fusion");

    MainWindow *w = new MainWindow();

    const QStringList files = {"main.cpp", "Window.h", "EditorWindow.cpp", "styles.css", "Wavy-Lox.txt"};
    for (const QString &fileName : files)
    {
        auto *win = new EditorWindow();
        win->initializeContent(fileName);
        w->splitter()->addWidget(win);
    }

    w->setGeometry(100, 100, 800, 600);
    w->showNormal();

    return a.exec();
}