#ifndef VIRTUAL_SPLITTER_H
#define VIRTUAL_SPLITTER_H

#include "virtual_group.h"

#include <QSplitter>

class VirtualSplitter : public QSplitter
{
    Q_OBJECT

public:
    explicit VirtualSplitter(Qt::Orientation orientation, QWidget *parent = nullptr);

    void cleanupStructure() { cleanupStructure(this); }
    static void cleanupStructure(QSplitter *splitter);
    static void cleanupStructure(VirtualGroup *group);

    bool allowDrop() const { return m_allowDrop; }
    void setAllowDrop(bool allow) { m_allowDrop = allow; }

    bool allowRemove() const { return m_allowRemove; }
    void setAllowRemove(bool allow) { m_allowRemove = allow; }

protected:
    bool m_allowDrop = true;
    bool m_allowRemove = true;
};

#endif
