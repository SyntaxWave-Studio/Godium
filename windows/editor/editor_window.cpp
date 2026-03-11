#include "editor_window.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QShortcut>
#include <QFileInfo>

EditorWindow::EditorWindow(QWidget *parent) : VirtualWindow(parent) {}

VirtualWindow *EditorWindow::createNew() { return new EditorWindow(); }

void EditorWindow::initializeContent(const QVariant &data)
{
    QString path = data.toString();
    setupLayout(path);
    setupShortcuts();

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_editor->setPlainText(file.readAll());
        file.close();
    }

    QString fileName = QFileInfo(path).fileName();
    setProperty("filePath", path);
    setProperty("fileName", fileName);
    setProperty("isDirty", false);

    setTabTitle(fileName);
    setWindowTitle(fileName);
}

void EditorWindow::setupLayout(const QString &path)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QString displayPath = path;
    displayPath.replace("/", " > ");

    QLabel *breadcrumbs = new QLabel("  " + displayPath);
    breadcrumbs->setFixedHeight(25);
    breadcrumbs->setStyleSheet("background: #1e1e1e; color: #858585; font-size: 11px; border-bottom: 1px solid #2d2d2d;");

    m_editor = new CodeEditor(this);

    layout->addWidget(breadcrumbs);
    layout->addWidget(m_editor);

    connect(m_editor, &CodeEditor::textChanged, this, &EditorWindow::handleTextChanged);
}

void EditorWindow::setupShortcuts()
{
    QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);

    saveShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(saveShortcut, &QShortcut::activated, this, &EditorWindow::saveFile);
}

void EditorWindow::handleTextChanged()
{
    if (!property("isDirty").toBool())
    {
        setProperty("isDirty", true);
        setTabTitle(property("fileName").toString() + "*");

        emit tabTitleChanged(tabTitle());
    }
}

void EditorWindow::saveFile()
{
    QFile file(property("filePath").toString());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream(&file) << m_editor->toPlainText();
        file.close();

        setProperty("isDirty", false);
        setTabTitle(property("fileName").toString());

        emit tabTitleChanged(tabTitle());
    }
}