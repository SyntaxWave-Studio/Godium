#ifndef LAYOUTWINDOW_H
#define LAYOUTWINDOW_H

#include "virtual_group.h"
#include "virtual_splitter.h"
#include "frameless_window.h"

#include <QMainWindow>
#include <QVBoxLayout>

class LayoutWindow : public FramelessWindow
{
    Q_OBJECT

public:
    explicit LayoutWindow(VirtualWindow *virtualWindow, QWidget *parent = nullptr);

    VirtualSplitter *groupSplitter() const { return m_groupSplitter; }

protected:
    VirtualSplitter *m_groupSplitter;
};

#endif