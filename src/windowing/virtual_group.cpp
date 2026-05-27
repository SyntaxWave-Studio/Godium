#include "virtual_group.h"
#include "virtual_splitter.h"
#include "window_factory.h"
#include "layout_window.h"

#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QTabBar>
#include <QBitmap>
#include <QPainter>

VirtualGroup::VirtualGroup(VirtualWindow *virtualWindow, QWidget *parent) : QTabWidget(parent)
{
    setupUi();
    setupStyle();
    setupPreview();
    tabBar()->installEventFilter(this);

    if (virtualWindow)
        addWindow(virtualWindow);

    connect(this, &QTabWidget::tabCloseRequested, this, &VirtualGroup::onTabClosed);
}

void VirtualGroup::addWindow(VirtualWindow *window)
{
    addTab(window, window->tabTitle());
    setCurrentWidget(window);

    connect(window, &VirtualWindow::tabTitleChanged, this, [this, window](const QString &newTitle) {
        int idx = indexOf(window);
        if (idx != -1)
            setTabText(idx, newTitle);
    });
}

void VirtualGroup::onTabClosed(int index)
{
    QWidget *w = widget(index);
    if (w)
    {
        removeTab(index);
        w->deleteLater();
    }

    VirtualSplitter::cleanupStructure(this);
}

void VirtualGroup::setupUi()
{
    setAcceptDrops(true);
    setTabsClosable(true);
    setMovable(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(50, 50);
    setDocumentMode(true);

    setUsesScrollButtons(true);
    setUsesScrollButtons(false);
    tabBar()->setElideMode(Qt::ElideRight);
}

void VirtualGroup::setupStyle()
{
    setStyleSheet(
        "QTabWidget::pane { border: none; background: #1e1e1e; }"
        "QTabBar { background: #181818; qproperty-drawBase: 0; }"
        "QTabBar::tab { background: #2d2d2d; color: #969696; padding: 4px 8px; margin: 0px; min-width: 80px; border-right: 1px solid #181818; }"
        "QTabBar::tab:first { margin-left: 0px; padding-left: 10px; }"
        "QTabBar::tab:selected { background: #1e1e1e; color: #ffffff; border-top: 1px solid #007acc; }"
        "QTabBar::close-button:hover { subcontrol-origin: margin; subcontrol-position: right; margin-right: 4px; width: 16px; }"
    );
}

void VirtualGroup::setupPreview()
{
    preview = new QRubberBand(QRubberBand::Rectangle, window());
    preview->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    preview->setAttribute(Qt::WA_TransparentForMouseEvents);
    preview->setStyleSheet("background-color: rgba(0, 122, 204, 30); border: 1px solid #007acc;");
}

void VirtualGroup::resizeEvent(QResizeEvent *event)
{
    QTabWidget::resizeEvent(event);

    QBitmap map(size());
    map.fill(Qt::color0);

    QPainter painter(&map);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::color1);
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 12, 12);
    painter.end();

    setMask(map);
}

bool VirtualGroup::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == tabBar() && qobject_cast<VirtualGroup *>(obj->parent()))
    {
        if (e->type() == QEvent::MouseButtonPress)
            dragStartPos = static_cast<QMouseEvent *>(e)->pos();
        else if (e->type() == QEvent::MouseMove)
        {
            QMouseEvent *me = static_cast<QMouseEvent *>(e);
            if (me->buttons() & Qt::LeftButton && (me->pos() - dragStartPos).manhattanLength() > 10)
            {
                int idx = tabBar()->tabAt(dragStartPos);
                if (idx != -1)
                {
                    startDrag(idx);
                    return true;
                }
            }
        }
    }
    return QTabWidget::eventFilter(obj, e);
}

void VirtualGroup::startDrag(int idx)
{
    VirtualWindow *window = qobject_cast<VirtualWindow *>(widget(idx));
    if (!window)
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData();

    mime->setData("application/x-virtualwindow-ptr", QByteArray::number(reinterpret_cast<quintptr>(window)));
    mime->setText(tabText(idx));

    drag->setMimeData(mime);

    Qt::DropAction result = drag->exec(Qt::MoveAction);
    if (result == Qt::IgnoreAction)
    {
        int realIdx = indexOf(window);
        if (realIdx != -1)
            removeTab(realIdx);

        QString title = tabText(idx);

        LayoutWindow *floatingWin = new LayoutWindow(window);

        int baseSize = this->height();
        int side = std::clamp(static_cast<int>(baseSize * 0.85), 500, 1200);

        floatingWin->resize(side, side);
        floatingWin->move(QCursor::pos() - QPoint(side / 2, 30));

        floatingWin->setWindowTitle(title);
        floatingWin->show();
    }

    VirtualSplitter::cleanupStructure(this);
}

void VirtualGroup::dragEnterEvent(QDragEnterEvent *e)
{
    const QMimeData *mime = e->mimeData();
    if (!mime->hasFormat("application/x-virtualwindow-ptr") && !mime->hasUrls())
    {
        e->ignore();
        preview->hide();
        return;
    }

    VirtualSplitter *parentSplitter = qobject_cast<VirtualSplitter *>(parentWidget());
    if (!parentSplitter || !parentSplitter->allowDrop())
    {
        e->ignore();
        preview->hide();
        return;
    }

    e->acceptProposedAction();
    preview->setGeometry(calculatePreviewRect(e->position().toPoint()));
    preview->show();
}

void VirtualGroup::dragMoveEvent(QDragMoveEvent *e)
{
    const QMimeData *mime = e->mimeData();
    if (mime->hasFormat("application/x-virtualwindow-ptr") || mime->hasUrls())
    {
        VirtualSplitter *parentSplitter = qobject_cast<VirtualSplitter *>(parentWidget());
        if (!parentSplitter || !parentSplitter->allowDrop())
        {
            e->ignore();
            preview->hide();
            return;
        }

        e->acceptProposedAction();
        preview->setGeometry(calculatePreviewRect(e->position().toPoint()));
        preview->show();
        return;
    }

    e->ignore();
    preview->hide();
}

void VirtualGroup::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e);
    preview->hide();
}

void VirtualGroup::dropEvent(QDropEvent *e)
{
    preview->hide();

    const QMimeData *mime = e->mimeData();
    if (mime->hasFormat("application/x-virtualwindow-ptr"))
    {
        quintptr ptr = mime->data("application/x-virtualwindow-ptr").toULongLong();
        VirtualWindow *window = reinterpret_cast<VirtualWindow *>(ptr);

        int zone = determineDropZone(e->position().toPoint());

        int tabIndex = indexOf(window);
        if (tabIndex != -1)
        {
            bool isFromList = (count() > 1);
            if ((isFromList && zone == ZoneCenter) || (!isFromList))
            {
                e->setDropAction(Qt::MoveAction);
                e->accept();
                return;
            }
        }

        handleDrop(zone, window);

        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else if (mime->hasUrls())
    {
        QList<QUrl> urls = mime->urls();
        QPoint pos = e->position().toPoint();
        int zone = determineDropZone(pos);

        VirtualWindow *(*createWindowSafe)(const QUrl &) = [](const QUrl &url) -> VirtualWindow *
        {
            VirtualWindow *window = WindowFactory::createWindowFromUrl(url);
            if (!window)
                qWarning() << "WindowFactory failed to create a window for path:" << url.toLocalFile();
            return window;
        };

        VirtualGroup *targetGroup = nullptr;
        int startIndex = 0;

        for (; startIndex < urls.size(); ++startIndex)
        {
            VirtualWindow *newWin = createWindowSafe(urls[startIndex]);
            if (!newWin)
                continue;

            targetGroup = handleDrop(zone, newWin);
            break;
        }

        if (!targetGroup)
        {
            e->ignore();
            return;
        }

        for (int i = startIndex + 1; i < urls.size(); ++i)
        {
            VirtualWindow *newWin = createWindowSafe(urls[startIndex]);
            if (!newWin)
                continue;

            targetGroup->addWindow(newWin);
        }

        e->setDropAction(Qt::CopyAction);
        e->accept();
    }
    else
        e->ignore();
}

QRect VirtualGroup::calculatePreviewRect(const QPoint &pos) const
{
    int w = width();
    int h = height();

    if (pos.x() < w * 0.2)
        return QRect(0, 0, w / 2, h);
    if (pos.x() > w * 0.8)
        return QRect(w / 2, 0, w / 2, h);
    if (pos.y() < h * 0.2)
        return QRect(0, 0, w, h / 2);
    if (pos.y() > h * 0.8)
        return QRect(0, h / 2, w, h / 2);

    return QRect(0, 0, w, h);
}

int VirtualGroup::determineDropZone(const QPoint &pos) const
{
    int w = width();
    int h = height();

    if (pos.x() < w * 0.2)
        return ZoneLeft;
    if (pos.x() > w * 0.8)
        return ZoneRight;
    if (pos.y() < h * 0.2)
        return ZoneTop;
    if (pos.y() > h * 0.8)
        return ZoneBottom;

    return ZoneCenter;
}

VirtualGroup *VirtualGroup::handleDrop(int zone, VirtualWindow *window)
{
    switch (zone)
    {
    case ZoneLeft:
        return splitWindow(Qt::Horizontal, true, window);
    case ZoneRight:
        return splitWindow(Qt::Horizontal, false, window);
    case ZoneTop:
        return splitWindow(Qt::Vertical, true, window);
    case ZoneBottom:
        return splitWindow(Qt::Vertical, false, window);
    default:
        addWindow(window);
        return this;
    }
}

VirtualGroup *VirtualGroup::splitWindow(Qt::Orientation orientation, bool insertBefore, VirtualWindow *window)
{
    VirtualSplitter *parentSplitter = qobject_cast<VirtualSplitter *>(parentWidget());
    VirtualGroup *newGroup = new VirtualGroup(window);

    if (!parentSplitter)
    {
        addWindow(window);
        return this;
    }

    int idx = parentSplitter->indexOf(this);
    QList<int> parentSizes = parentSplitter->sizes();
    int currentSpace = parentSizes[idx];

    if (parentSplitter->orientation() == orientation)
    {
        parentSplitter->insertWidget(insertBefore ? idx : idx + 1, newGroup);

        parentSizes[idx] = currentSpace / 2;
        parentSizes.insert(insertBefore ? idx : idx + 1, currentSpace / 2);
        parentSplitter->setSizes(parentSizes);
    }
    else
    {
        VirtualSplitter *newSplitter = new VirtualSplitter(orientation);
        parentSplitter->replaceWidget(idx, newSplitter);

        if (insertBefore)
        {
            newSplitter->addWidget(newGroup);
            newSplitter->addWidget(this);
        }
        else
        {
            newSplitter->addWidget(this);
            newSplitter->addWidget(newGroup);
        }

        newSplitter->setSizes({currentSpace / 2, currentSpace / 2});
        parentSplitter->setSizes(parentSizes);
    }

    return newGroup;
}
