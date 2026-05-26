#ifndef LAYOUTWINDOW_H
#define LAYOUTWINDOW_H

#include "virtual_group.h"
#include "virtual_splitter.h"

#include <QMainWindow>
#include <QVBoxLayout>

class LayoutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LayoutWindow(VirtualGroup *virtualGroup, QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setStyleSheet("QLayoutWindow { background: #0b0b0c; }");
        setAttribute(Qt::WA_DeleteOnClose);

        QWidget *central = new QWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(central);
        layout->setContentsMargins(6, 6, 6, 6);
        layout->setSpacing(0);

        mainSplitter = new VirtualSplitter(Qt::Horizontal, central);

        layout->addWidget(mainSplitter);
        setCentralWidget(central);

        mainSplitter->addWidget(virtualGroup);
    }

    VirtualSplitter *splitter() const { return mainSplitter; }

private:
    VirtualSplitter *mainSplitter;
};

#endif