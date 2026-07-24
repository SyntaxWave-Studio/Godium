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
    static void cleanupStructure(VirtualSplitter *splitter);
    static void cleanupStructure(VirtualGroup *group);

    bool allowRemove() const { return m_allowRemove; }
    void setAllowRemove(bool allow) { m_allowRemove = allow; }

    bool saveGroup() const { return m_saveGroup; }
    void setSaveGroup(bool save) { m_saveGroup = save; }

private:
    bool m_allowRemove = true;
    bool m_saveGroup = false;
};

#endif
