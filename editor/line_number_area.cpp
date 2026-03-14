#include "line_number_area.h"
#include "code_editor.h"

#include <QPainter>

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor), m_editor(editor) 
{
    setFont(m_editor->font());
    connect(m_editor, &CodeEditor::updateRequest, this, &LineNumberArea::updateLineNumberArea);
}

void LineNumberArea::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        scroll(0, dy);
    else
        update(0, m_editor->rect().y(), width(), m_editor->rect().height());
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QColor("#181818"));

    painter.setPen(QColor("#333333"));
    painter.drawLine(width() - 1, 0, width() - 1, height());

    QTextBlock block = m_editor->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(m_editor->blockBoundingGeometry(block).translated(m_editor->contentOffset()).top());
    int bottom = top + qRound(m_editor->blockBoundingRect(block).height());
    int cursorBlockNumber = m_editor->textCursor().blockNumber();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            painter.setPen(QColor("#858585"));
            painter.drawText(0, top, width() - 5, bottom - top, Qt::AlignRight | Qt::AlignTop, QString::number(blockNumber + 1));
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(m_editor->blockBoundingRect(block).height());
        blockNumber++;
    }
}
