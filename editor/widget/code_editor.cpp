#include "code_editor.h"

#include <QTextBlock>
#include <QScrollBar>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    setAcceptDrops(false);
    viewport()->setAcceptDrops(false);
    document()->setDocumentMargin(0);

    setLineWrapMode(QPlainTextEdit::WidgetWidth);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, &CodeEditor::updateRequest, this, [this](const QRect &rect, int dy) {
        lineNumberArea->updateArea(rect, dy);
        if (rect.contains(viewport()->rect())) {
            syncLayout();
        } 
    });
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    syncLayout();
    highlightCurrentLine();

    setFrameStyle(QFrame::NoFrame);
    setStyleSheet("background: #1e1e1e; color: #d4d4d4; font-family: 'Consolas', 'Monaco', monospace; font-size: 13px;");

    verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical { background: #1e1e1e; width: 14px; margin: 0px; border: none; }"
        "QScrollBar::handle:vertical { background: rgba(121, 121, 121, 0.4); min-height: 20px; margin: 2px 3px; border-radius: 0px; }"
        "QScrollBar::handle:vertical:hover { background: rgba(121, 121, 121, 0.7); }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; background: none; border: none; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
    );
}

void CodeEditor::syncLayout()
{
    int areaWidth = lineNumberArea->calculateWidth(blockCount());
    setViewportMargins(areaWidth + 3, 0, 3, 0);

    QRect cr = contentsRect();
    QRect areaRect(cr.left(), cr.top(), areaWidth, cr.height());
    lineNumberArea->updateGeometry(areaRect);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor("#2d2d2d"));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
    lineNumberArea->update();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    syncLayout();
}