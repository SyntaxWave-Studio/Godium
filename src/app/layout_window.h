#ifndef LAYOUTWINDOW_H
#define LAYOUTWINDOW_H

#include "virtual_group.h"
#include "virtual_splitter.h"
#include "control_bar.h"
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
    FramelessWindow *m_FramelessWindow;
    QSplitter *m_menuSplitter;
    ControlBar *m_controlBar;
    
    QWidget *m_contentWidget;
    QVBoxLayout *m_layout;
    VirtualSplitter *m_groupSplitter;
};

#endif