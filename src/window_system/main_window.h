#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "virtual_splitter.h"

#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setStyleSheet("QMainWindow { background: #0b0b0c; }");
        setAttribute(Qt::WA_DeleteOnClose);

        QWidget *central = new QWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(central);
        layout->setContentsMargins(6, 6, 6, 6);
        layout->setSpacing(0);

        mainSplitter = new VirtualSplitter(Qt::Horizontal, central);

        layout->addWidget(mainSplitter->container());
        setCentralWidget(central);
    }

    VirtualSplitter *splitter() const { return mainSplitter; }

private:
    VirtualSplitter *mainSplitter;
};

#endif
