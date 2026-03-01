#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "VirtualWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

class EditorWindow : public VirtualWindow
{
    Q_OBJECT
public:
    using VirtualWindow::VirtualWindow;
    VirtualWindow *createNew() override;
    void initializeContent(const QVariant &data) override;
};

#endif
