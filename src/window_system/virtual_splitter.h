#ifndef VIRTUAL_SPLITTER_H
#define VIRTUAL_SPLITTER_H

#include <QSplitter>

class VirtualSplitter : public QSplitter
{
    Q_OBJECT

public:
    explicit VirtualSplitter(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QSplitter(orientation, parent)
    {
        setHandleWidth(6);
        setStyleSheet(
            "QSplitter { background: transparent; border: none; }"
            "QSplitter::handle { background: transparent; }"
        );
    }

    QWidget *container() const { return const_cast<VirtualSplitter *>(this); }
};

#endif
