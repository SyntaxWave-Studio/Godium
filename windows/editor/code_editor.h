#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QWidget>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class LineNumberArea;
public:
    explicit CodeEditor(QWidget *parent = nullptr);
private: 
    int lineNumberAreaWidth() const;
    void updateLineNumberAreaWidth(int newBlockCount);

    void highlightCurrentLine();
    void resizeEvent(QResizeEvent *event) override;

    QWidget *lineNumberArea;
};

#endif
