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
    explicit LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent = nullptr);

    VirtualSplitter *groupSplitter() const { return m_groupSplitter; }

protected:
    QWidget *m_central;
    QVBoxLayout *m_layout;
    VirtualSplitter *m_groupSplitter;
};

#endif