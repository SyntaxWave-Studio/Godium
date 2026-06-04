#ifndef LAYOUT_WINDOW_H
#define LAYOUT_WINDOW_H

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

    //Test void; will be deleted
    void addWidgetToGroupSplitter(QWidget *widget) { m_groupSplitter->addWidget(widget); }

protected:
    QVBoxLayout *groupLayout() const { return m_groupLayout; }
    VirtualSplitter *groupSplitter() const { return m_groupSplitter; }

    void changeGroupParent(QWidget *parent);

private: 
    QWidget *m_groupWidget;
    QVBoxLayout *m_groupLayout;
    VirtualSplitter *m_groupSplitter;
};

#endif