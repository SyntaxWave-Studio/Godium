#include "MainWindow.h"
#include "EditorWindow.h" 
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

    const QStringList files = {"main.cpp", "Window.h", "EditorWindow.cpp", "styles.css", "Wavy-Lox.txt"};
    for (const QString &fileName : files)
    {
        EditorWindow *win = new EditorWindow();
        win->initializeContent(fileName);
        w->splitter()->addWidget(win);
    }

    w->showNormal();
    return a.exec();
}