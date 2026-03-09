#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "VirtualWindow.h"
#include <QPlainTextEdit>
#include <QShortcut>

class EditorWindow : public VirtualWindow
{
    Q_OBJECT
public:
    explicit EditorWindow(QWidget *parent = nullptr);

    VirtualWindow *createNew() override;
    void initializeContent(const QVariant &data) override;

private:
    void handleTextChanged();
    void saveFile();

    QPlainTextEdit *m_editor = nullptr;
};

#endif
