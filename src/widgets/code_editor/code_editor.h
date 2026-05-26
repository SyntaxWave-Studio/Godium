#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include "line_number_area.h"

#include <QWidget>
#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    
private:
    void syncLayout(); 
    void highlightCurrentLine();
    void resizeEvent(QResizeEvent *event) override;

    LineNumberArea *lineNumberArea;
};

#endif
