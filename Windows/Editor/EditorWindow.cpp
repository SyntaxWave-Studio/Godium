#include "EditorWindow.h"

VirtualWindow *EditorWindow::createNew()
{
    return new EditorWindow();
}

void EditorWindow::initializeContent(const QVariant &data)
{
    QString filePath = data.toString();
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *breadcrumbs = new QLabel("  " + filePath + " > ...");
    breadcrumbs->setFixedHeight(22);
    breadcrumbs->setStyleSheet("background: #1e1e1e; color: #858585; font-size: 11px;");

    QLabel *editorText = new QLabel();
    editorText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    editorText->setStyleSheet("background: #1e1e1e; color: #d4d4d4; font-family: 'Consolas', monospace; padding: 10px; border-top: 1px solid #252526;");
    editorText->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString content = in.readAll();
        editorText->setText(content);
        file.close();
    }
    else
    {
        editorText->setText("Failed to open file: " + filePath);
    }

    layout->addWidget(breadcrumbs);
    layout->addWidget(editorText);

    addTab(container, fileName);
}
