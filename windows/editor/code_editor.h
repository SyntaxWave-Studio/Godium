#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);

    int lineNumberAreaWidth();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);
    void highlightCurrentLine();

    void resizeEvent(QResizeEvent *event) override;
    void lineNumberAreaPaintEvent(QPaintEvent *event);

private:
    QWidget *lineNumberArea;
};

#endif
