#include "line_number_area.h"

#include <QPainter>
#include <QAbstractTextDocumentLayout>

LineNumberArea::LineNumberArea(QWidget *parent) : QWidget(parent)
{
    setFont(parent->font());
}

int LineNumberArea::calculateWidth(int blockCount) const
{
    int digits = 1;
    int max = qMax(1, blockCount);
    while (max >= 10)
    {
        max /= 10;
        digits++;
    }
    return 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

void LineNumberArea::updateGeometry(const QRect &rect)
{
    setGeometry(rect);
}

void LineNumberArea::updateArea(const QRect &rect, int dy)
{
    if (dy)
        scroll(0, dy);
    else
        update(0, rect.y(), width(), rect.height());
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    QPlainTextEdit *editor = qobject_cast<QPlainTextEdit *>(parentWidget());
    if (!editor)
        return;

    QPainter painter(this);
    painter.setFont(editor->font());

    painter.fillRect(event->rect(), QColor("#181818"));
    painter.setPen(QColor("#333333"));
    painter.drawLine(width() - 1, 0, width() - 1, height());

    QTextCursor topCursor = editor->cursorForPosition(QPoint(0, 0));
    QTextBlock block = topCursor.block();
    int blockNumber = block.blockNumber();
    int top = editor->cursorRect(topCursor).top();

    QAbstractTextDocumentLayout *layout = editor->document()->documentLayout();
    int bottom = top + qRound(layout->blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            painter.setPen(QColor("#858585"));
            painter.drawText(0, top, width() - 5, bottom - top,
                Qt::AlignRight | Qt::AlignTop, QString::number(blockNumber + 1));
        }

        block = block.next();
        top = bottom;
        if (block.isValid())
        {
            bottom = top + qRound(layout->blockBoundingRect(block).height());
        }
        blockNumber++;
    }
}
