#include "main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Godium");
    a.setApplicationDisplayName("Godium");
    a.setOrganizationName("SyntaxWaveStudio");

    a.setQuitOnLastWindowClosed(true);
    a.setStyle("Fusion");

    MainWindow *w = new MainWindow();
    
    w->showNormal();
    return a.exec();
}
