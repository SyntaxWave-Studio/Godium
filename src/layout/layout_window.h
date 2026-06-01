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

    //Testing void; will be deleted
    void addWidgetToGroupSplitter(QWidget *widget) { m_groupSplitter->addWidget(widget); }

protected:
    VirtualSplitter *groupSplitter() const { return m_groupSplitter; }
    QVBoxLayout *groupLayout() const { return m_groupLayout; }

    void changeGroupParent(QWidget *parent);

private: 
    QWidget *m_groupWidget;
    QVBoxLayout *m_groupLayout;
    VirtualSplitter *m_groupSplitter;
};

#endif