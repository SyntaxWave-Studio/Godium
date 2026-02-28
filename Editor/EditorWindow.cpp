#include "EditorWindow.h"
#include <QLabel>
#include <QVBoxLayout>

VirtualWindow *EditorWindow::createNew()
{
    return new EditorWindow();
}

void EditorWindow::initializeContent(const QVariant &data)
{
    QString fileName = data.toString();

    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *breadcrumbs = new QLabel("  " + fileName + " > ...");
    breadcrumbs->setFixedHeight(22);
    breadcrumbs->setStyleSheet("background: #1e1e1e; color: #858585; font-size: 11px;");

    QLabel *editorText = new QLabel("Здесь какой-то умный код.");
    editorText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    editorText->setStyleSheet("background: #1e1e1e; color: #d4d4d4; font-family: 'Consolas', monospace; padding: 10px; border-top: 1px solid #252526;");

    layout->addWidget(breadcrumbs);
    layout->addWidget(editorText);

    addTab(container, fileName);
}
