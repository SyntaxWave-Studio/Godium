#include "code_editor.h"

#include <QPainter>
#include <QTextBlock>

class CodeEditor;

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor);
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *m_editor;
};
