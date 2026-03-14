#include "code_editor.h"

#include <QPainter>
#include <QTextBlock>

class CodeEditor;

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor);
private:
    void updateLineNumberArea(const QRect &rect, int dy);
    void paintEvent(QPaintEvent *event) override;

    CodeEditor *m_editor;
};
