#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "VirtualWindow.h"
#include <QTabWidget>
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
    void setupUI(QWidget *container, QPlainTextEdit *editor, const QString &path);
    void updateTabStatus(QWidget *container);
    void handleTextChanged();
    void saveFile();
};

#endif