#include "virtual_splitter.h"

VirtualSplitter::VirtualSplitter(Qt::Orientation orientation, QWidget *parent, int width) : QSplitter(orientation, parent)
{
    setHandleWidth(width);
    setStyleSheet(
        "QSplitter { background: transparent; border: none; }"
        "QSplitter::handle { background: transparent; }"
    );
}

void VirtualSplitter::cleanupStructure(QSplitter *splitter)
{
    VirtualSplitter *vSplitter = qobject_cast<VirtualSplitter *>(splitter);
    if (!vSplitter || !vSplitter->allowRemove())
        return;

    QSplitter *parentSplitter = qobject_cast<QSplitter *>(splitter->parentWidget());
    if (splitter->count() == 0)
    {
        splitter->deleteLater();
        if (parentSplitter)
            cleanupStructure(parentSplitter);
    }
    else if (splitter->count() == 1 && parentSplitter)
    {
        QWidget *child = splitter->widget(0);
        int idx = parentSplitter->indexOf(splitter);
        parentSplitter->insertWidget(idx, child);
        splitter->deleteLater();
        cleanupStructure(parentSplitter);
    }
}

void VirtualSplitter::cleanupStructure(VirtualGroup *group)
{
    if (group->count() == 0)
    {
        QSplitter *parentSplitter = qobject_cast<QSplitter *>(group->parentWidget());

        group->setParent(nullptr);
        group->deleteLater();

        if (parentSplitter)
            VirtualSplitter::cleanupStructure(parentSplitter);
    }
}