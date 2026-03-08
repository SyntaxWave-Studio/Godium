#include "EditorWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>

EditorWindow::EditorWindow(QWidget *parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

VirtualWindow *EditorWindow::createNew()
{
    return new EditorWindow();
}

void EditorWindow::initializeContent(const QVariant &data)
{
    QString path = data.toString();
    if (path.isEmpty())
        return;

    QString fileName = QFileInfo(path).fileName();

    QWidget *container = new QWidget();
    QPlainTextEdit *editor = new QPlainTextEdit(container);

    container->setProperty("filePath", path);
    container->setProperty("fileName", fileName);
    container->setProperty("isDirty", false);

    setupUI(container, editor, path);

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        editor->setPlainText(file.readAll());
        file.close();
    }

    this->addTab(container, fileName);
    this->setCurrentWidget(container);

    editor->setFocus();
    editor->setAcceptDrops(false);

    if (!this->findChild<QShortcut *>("_saveShortcut"))
    {
        QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
        saveShortcut->setObjectName("_saveShortcut");
        saveShortcut->setContext(Qt::WidgetWithChildrenShortcut);
        connect(saveShortcut, &QShortcut::activated, this, &EditorWindow::saveFile);
    }

    connect(editor, &QPlainTextEdit::textChanged, this, &EditorWindow::handleTextChanged);
}

void EditorWindow::setupUI(QWidget *container, QPlainTextEdit *editor, const QString &path)
{
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QString breadcrumbsText = "  " + path;
    breadcrumbsText.replace("/", " > ");
    QLabel *breadcrumbs = new QLabel(breadcrumbsText);
    breadcrumbs->setFixedHeight(25);
    breadcrumbs->setStyleSheet("background: #1e1e1e; color: #858585; font-size: 11px; border-bottom: 1px solid #2d2d2d;");

    editor->setFrameStyle(QFrame::NoFrame);
    editor->setStyleSheet("background: #1e1e1e; color: #d4d4d4; font-family: monospace; padding-left: 10px;");

    layout->addWidget(breadcrumbs);
    layout->addWidget(editor);
}

void EditorWindow::updateTabStatus(QWidget *container)
{
    int idx = this->indexOf(container);
    if (idx != -1)
    {
        QString name = container->property("fileName").toString();
        bool dirty = container->property("isDirty").toBool();
        this->setTabText(idx, name + (dirty ? " ●" : ""));
    }
}

void EditorWindow::handleTextChanged()
{
    std::cout << "Text Changed" << std::endl;

    QPlainTextEdit *editor = qobject_cast<QPlainTextEdit *>(sender());
    if (!editor)
        return;

    int idx = -1;
    for (int i = 0; i < this->count(); ++i)
    {
        if (this->widget(i)->isAncestorOf(editor))
        {
            idx = i;
            break;
        }
    }

    if (idx != -1)
    {
        QWidget *container = this->widget(idx);
        if (!container->property("isDirty").toBool())
        {
            container->setProperty("isDirty", true);
            QString name = container->property("fileName").toString();
            this->setTabText(idx, name + " ●");
        }
    }
}

void EditorWindow::saveFile()
{
    QWidget *container = this->currentWidget();
    if (!container)
        return;

    QPlainTextEdit *editor = container->findChild<QPlainTextEdit *>();
    if (!editor)
        return;

    QString path = container->property("filePath").toString();
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << editor->toPlainText();
        file.close();

        container->setProperty("isDirty", false);
        updateTabStatus(container);
        qDebug() << "Saved to:" << path;
    }
}
