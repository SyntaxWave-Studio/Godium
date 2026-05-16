#ifndef LINE_NUMBER_AREA_H
#define LINE_NUMBER_AREA_H

#include <QWidget>
#include <QPainter>
#include <QTextBlock>
#include <QPlainTextEdit>

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(QWidget *parent = nullptr);

    int calculateWidth(int blockCount) const;
    void updateArea(const QRect &rect, int dy);
    void updateGeometry(const QRect &rect);

private:
    void paintEvent(QPaintEvent *event) override;
};

#endif