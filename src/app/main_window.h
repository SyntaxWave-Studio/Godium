#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "virtual_window.h"
#include "layout_window.h"

#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow : public LayoutWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void setDockWindow(VirtualWindow *dock);

protected:
    VirtualSplitter *m_mainSplitter;
    VirtualWindow *m_dockPanel = nullptr;
};

#endif
