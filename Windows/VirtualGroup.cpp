#include "VirtualGroup.h"
#include "MainWindow.h"
#include "WindowFactory.h"

VirtualGroup::VirtualGroup(QWidget *parent) : QTabWidget(parent)
{
    setupUi();
    setupStyle();
    setupPreview();
    tabBar()->installEventFilter(this);

    connect(this, &QTabWidget::tabCloseRequested, this, &VirtualGroup::handleClose);
}

void VirtualGroup::setupUi()
{
    setAcceptDrops(true);
    setTabsClosable(true);
    setMovable(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(50, 50);
    setDocumentMode(true);
}

void VirtualGroup::setupStyle()
{
    setStyleSheet(
        "QTabWidget::pane { border: none; background: #1e1e1e; }"
        "QTabBar { background: #181818; qproperty-drawBase: 0; left: 0px; margin-left: 0px; }"
        "QTabBar::tab { background: #2d2d2d; color: #969696; padding: 4px 8px; margin: 0px; min-width: 80px; border-right: 1px solid #181818; }"
        "QTabBar::tab:first { margin-left: 0px; padding-left: 10px; }"
        "QTabBar::tab:selected { background: #1e1e1e; color: #ffffff; border-top: 1px solid #007acc; }"
        "QTabBar::close-button:hover { subcontrol-origin: margin; subcontrol-position: right; margin-right: 4px; width: 16px; }"
    );
}

void VirtualGroup::setupPreview()
{
    preview = new QRubberBand(QRubberBand::Rectangle, this);
    preview->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    preview->setAttribute(Qt::WA_TransparentForMouseEvents);
    preview->setStyleSheet("background-color: rgba(0, 122, 204, 30); border: 1px solid #007acc;");
}

void VirtualGroup::addWindow(VirtualWindow *window, const QString &title)
{
    addTab(window, title);
    setCurrentWidget(window);

    connect(window, &VirtualWindow::tabTitleChanged, this, [this, window](const QString &newTitle) {
        int idx = indexOf(window);
        if (idx != -1)
            setTabText(idx, newTitle); 
    }, Qt::UniqueConnection);
}

void VirtualGroup::handleClose(int index)
{
    QWidget *w = widget(index);
    if (w)
    {
        removeTab(index);
        w->deleteLater();
    }
    checkEmptyAndCleanup();
}

bool VirtualGroup::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == tabBar())
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
        createFloatingWindow(window, tabText(idx));

    checkEmptyAndCleanup();
}

void VirtualGroup::createFloatingWindow(VirtualWindow *window, const QString &title)
{
    int realIdx = indexOf(window);
    if (realIdx != -1)
        removeTab(realIdx);

    MainWindow *floatingMainWin = new MainWindow();
    VirtualGroup *newGroup = new VirtualGroup();
    newGroup->addWindow(window, title);

    floatingMainWin->splitter()->addWidget(newGroup);

    int baseSize = this->height();
    int side = std::clamp(static_cast<int>(baseSize * 0.85), 500, 1200);

    floatingMainWin->resize(side, side);
    floatingMainWin->move(QCursor::pos() - QPoint(side / 2, 30));

    floatingMainWin->setWindowTitle(title);
    floatingMainWin->show();
}

void VirtualGroup::checkEmptyAndCleanup()
{
    if (count() > 0)
        return;

    QWidget *topLevel = window();
    QSplitter *parentSplitter = qobject_cast<QSplitter *>(parentWidget());

    setParent(nullptr);
    this->deleteLater();

    if (parentSplitter)
        cleanupStructure(parentSplitter);

    QPointer<QWidget> safeTopLevel = topLevel;
    QTimer::singleShot(0, [safeTopLevel]() {
        if (!safeTopLevel)
            return;

        QList<VirtualGroup *> remainingGroups = safeTopLevel->findChildren<VirtualGroup *>();

        if (remainingGroups.isEmpty())
            safeTopLevel->close();
    });
}

void VirtualGroup::cleanupStructure(QSplitter *splitter)
{
    if (!splitter)
        return;

    QSplitter *parentSplitter = qobject_cast<QSplitter *>(splitter->parentWidget());

    if (splitter->count() == 0)
    {
        QWidget *topLevel = splitter->window();
        splitter->deleteLater();

        if (parentSplitter)
            cleanupStructure(parentSplitter);
        else if (topLevel && qobject_cast<QMainWindow *>(topLevel))
        {
            int groupCount = topLevel->findChildren<VirtualGroup *>().count();
            if (groupCount == 0 && QApplication::topLevelWidgets().count() > 1)
                topLevel->close();
        }
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

void VirtualGroup::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasFormat("application/x-virtualwindow-ptr") || e->mimeData()->hasUrls())
        e->acceptProposedAction();
    else
    {
        e->ignore();
        preview->hide();
    }
}

void VirtualGroup::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasFormat("application/x-virtualwindow-ptr") || e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
        preview->setGeometry(calculatePreviewRect(e->position().toPoint()));
        preview->show();
    }
    else
    {
        e->ignore();
        preview->hide();
    }
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

        if (!window)
        {
            e->ignore();
            return;
        }

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

        handleDrop(zone, window, window->windowTitle());

        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else if (mime->hasUrls())
    {
        QList<QUrl> urls = mime->urls();
        QPoint pos = e->position().toPoint();
        int zone = determineDropZone(pos);

        for (const QUrl &url : urls)
        {
            VirtualWindow *newWin = WindowFactory::createWindowFromUrl(url);
            if (!newWin)
            {
                qWarning() << "WindowFactory failed to create a window for path:" << url.toLocalFile();
                continue;
            }

            handleDrop(zone, newWin, newWin->windowTitle());
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

void VirtualGroup::handleDrop(int zone, VirtualWindow *window, const QString &title)
{
    switch (zone)
    {
    case ZoneLeft:
        splitWindow(Qt::Horizontal, true, window, title);
        break;
    case ZoneRight:
        splitWindow(Qt::Horizontal, false, window, title);
        break;
    case ZoneTop:
        splitWindow(Qt::Vertical, true, window, title);
        break;
    case ZoneBottom:
        splitWindow(Qt::Vertical, false, window, title);
        break;
    default:
        addWindow(window, title);
        break;
    }
}

void VirtualGroup::splitWindow(Qt::Orientation orientation, bool insertBefore, VirtualWindow *window, const QString &title)
{
    QSplitter *parentSplitter = qobject_cast<QSplitter *>(parentWidget());
    VirtualGroup *newGroup = new VirtualGroup();
    newGroup->addWindow(window, title);

    if (!parentSplitter)
    {
        addWindow(window, title);
        return;
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
        QSplitter *newSplitter = new QSplitter(orientation);
        newSplitter->setHandleWidth(1);
        newSplitter->setStyleSheet("QSplitter::handle { background: #252526; }");

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
}
