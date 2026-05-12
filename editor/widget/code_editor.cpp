#include "code_editor.h"
#include "line_number_area.h"

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

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
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

int CodeEditor::lineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        digits++;
    }
    return 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

void CodeEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth() + 3, 0, 3, 0);
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
    updateLineNumberAreaWidth(0);
    
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}