#include "main_window.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Godium");
    a.setApplicationDisplayName("Godium");
    a.setOrganizationName("SyntaxWaveStudio");

    a.setQuitOnLastWindowClosed(true);
    a.setStyle("Fusion");

    QString baseDir = QCoreApplication::applicationDirPath();
    QString relativePath = (CONFIG_PATH && strlen(CONFIG_PATH) > 0) ? QString(CONFIG_PATH) : QString("config");
    QString fullConfigPath = QDir(baseDir).filePath(relativePath);
    qDebug() << "Config folder path:" << fullConfigPath;

    MainWindow *w = new MainWindow();
    
    w->showNormal();
    return a.exec();
}
