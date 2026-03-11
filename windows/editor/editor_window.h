#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include "virtual_window.h"
#include "code_editor.h"

class EditorWindow : public VirtualWindow
{
    Q_OBJECT
public:
    explicit EditorWindow(QWidget *parent = nullptr);
    VirtualWindow *createNew() override;
    void initializeContent(const QVariant &data) override;
    
private:
    CodeEditor *m_editor = nullptr;

    void setupLayout(const QString &path);
    void setupShortcuts();

    void handleTextChanged();
    void saveFile();
};

#endif
