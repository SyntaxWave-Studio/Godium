#include "editor_window.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDebug>

EditorWindow::EditorWindow(QWidget *parent) : VirtualWindow(parent)
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
    setWindowTitle(fileName);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QString breadcrumbsText = "  " + path;
    breadcrumbsText.replace("/", " > ");
    QLabel *breadcrumbs = new QLabel(breadcrumbsText);
    breadcrumbs->setFixedHeight(25);
    breadcrumbs->setStyleSheet("background: #1e1e1e; color: #858585; font-size: 11px; border-bottom: 1px solid #2d2d2d;");

    m_editor = new QPlainTextEdit(this);
    m_editor->setFrameStyle(QFrame::NoFrame);
    m_editor->setStyleSheet("background: #1e1e1e; color: #d4d4d4; font-family: monospace; padding-left: 10px;");

    layout->addWidget(breadcrumbs);
    layout->addWidget(m_editor);

    setLayout(layout);

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_editor->setPlainText(file.readAll());
        file.close();
    }

    m_editor->setFocus();
    m_editor->setAcceptDrops(false);

    if (!findChild<QShortcut *>("_saveShortcut"))
    {
        QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
        saveShortcut->setObjectName("_saveShortcut");
        saveShortcut->setContext(Qt::WidgetWithChildrenShortcut);
        connect(saveShortcut, &QShortcut::activated, this, &EditorWindow::saveFile);
    }

    connect(m_editor, &QPlainTextEdit::textChanged, this, &EditorWindow::handleTextChanged);

    setProperty("filePath", path);
    setProperty("fileName", fileName);
    setProperty("isDirty", false);
}

void EditorWindow::handleTextChanged()
{
    if (!property("isDirty").toBool())
    {
        setProperty("isDirty", true);
        QString baseTitle = property("fileName").toString();
        setTabTitle(baseTitle + "*");
        emit tabTitleChanged(tabTitle());
    }
}

void EditorWindow::saveFile()
{
    QString path = property("filePath").toString();
    if (path.isEmpty() || !m_editor)
        return;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << m_editor->toPlainText();
        file.close();

        setProperty("isDirty", false);
        QString baseTitle = property("fileName").toString();
        setTabTitle(baseTitle);
        emit tabTitleChanged(tabTitle());

        qDebug() << "Saved to:" << path;
    }
}
