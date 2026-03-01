#include <VirtualWindow.h>
#include <MainWindow.h>
#include <DragData.h>

VirtualWindow::VirtualWindow(QWidget *parent) : QTabWidget(parent)
{
    setupUi();
    setupStyle();
    setupPreview();
    tabBar()->installEventFilter(this);

    connect(this, &QTabWidget::tabCloseRequested, this, &VirtualWindow::handleClose);
}

void VirtualWindow::setupUi()
{
    setAcceptDrops(true);
    setTabsClosable(true);
    setMovable(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(50, 50);
    setDocumentMode(true);
}

void VirtualWindow::setupStyle()
{
    setStyleSheet(
        "QTabWidget::pane { border: none; background: #1e1e1e; }"
        "QTabBar { background: #181818; qproperty-drawBase: 0; left: 0px; margin-left: 0px; }"
        "QTabBar::tab { background: #2d2d2d; color: #969696; padding: 4px 8px; margin: 0px; min-width: 80px; border-right: 1px solid #181818; }"
        "QTabBar::tab:first { margin-left: 0px; padding-left: 10px; }"
        "QTabBar::tab:selected { background: #1e1e1e; color: #ffffff; border-top: 1px solid #007acc; }"
        "QTabBar::close-button:hover { subcontrol-origin: margin; subcontrol-position: right; margin-right: 4px; width: 16px; }");
}

void VirtualWindow::setupPreview()
{
    preview = new QRubberBand(QRubberBand::Rectangle, this);
    preview->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    preview->setAttribute(Qt::WA_TransparentForMouseEvents);
    preview->setStyleSheet("background-color: rgba(0, 122, 204, 30); border: 1px solid #007acc;");
}

void VirtualWindow::handleClose(int index)
{
    QWidget *w = widget(index);
    if (w)
    {
        removeTab(index);
        w->deleteLater();
    }

    checkEmptyAndCleanup();
}

bool VirtualWindow::eventFilter(QObject *obj, QEvent *e)
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

QMimeData *VirtualWindow::createMimeData(QWidget *page, const QString &title)
{
    QMimeData *mime = new QMimeData;
    DragData dd;
    dd.widget = page;
    dd.sourceWindow = this;
    dd.title = title;
    mime->setData("application/x-widget-ptr", dd.serialize());
    mime->setText(title);
    return mime;
}

bool VirtualWindow::extractDragData(const QMimeData *mime, QWidget *&widget, VirtualWindow *&sourceWin)
{
    DragData dd = DragData::deserialize(mime);
    widget = dd.widget;
    sourceWin = dd.sourceWindow;
    return widget && sourceWin;
}

void VirtualWindow::createFloatingWindow(QWidget *page, const QString &title)
{
    int realIdx = indexOf(page);
    if (realIdx != -1)
        removeTab(realIdx);

    VirtualWindow *newWin = createNew();
    newWin->addTab(page, title);

    QWidget *floatingWin = new QWidget();
    floatingWin->resize(this->size());
    floatingWin->move(QCursor::pos() - QPoint(50, 10));

    QVBoxLayout *layout = new QVBoxLayout(floatingWin);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(newWin);

    floatingWin->show();
}

void VirtualWindow::checkEmptyAndCleanup()
{
    if (count() > 0)
        return;

    QWidget *topLevel = window();
    QSplitter *parentSplitter = qobject_cast<QSplitter *>(parentWidget());

    setParent(nullptr);
    this->deleteLater();

    if (parentSplitter)
        cleanupStructure(parentSplitter);

    QTimer::singleShot(0, [topLevel]() {
        if (!topLevel) return;

        auto remainingWindows = topLevel->findChildren<VirtualWindow *>();
    
        if (remainingWindows.isEmpty()) {
            topLevel->close();
        } 
    });
}

void VirtualWindow::cleanupStructure(QSplitter *splitter)
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
            int vwCount = topLevel->findChildren<VirtualWindow *>().count();
            qDebug() << "cleanupStructure: topLevel VirtualWindow count:" << vwCount;
            if (vwCount == 0 && QApplication::topLevelWidgets().count() > 1)
            {
                qDebug() << "cleanupStructure: closing topLevel window";
                topLevel->close();
            }
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

void VirtualWindow::startDrag(int idx)
{
    QWidget *page = widget(idx);
    QString title = tabText(idx);

    QDrag *drag = new QDrag(this);
    QMimeData *mime = createMimeData(page, title);
    drag->setMimeData(mime);

    Qt::DropAction result = drag->exec(Qt::MoveAction);

    if (result == Qt::IgnoreAction)
        createFloatingWindow(page, title);

    checkEmptyAndCleanup();
}

void VirtualWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();
}

void VirtualWindow::dragMoveEvent(QDragMoveEvent *e)
{
    e->acceptProposedAction();
    preview->setGeometry(calculatePreviewRect(e->position().toPoint()));
    preview->show();
}

void VirtualWindow::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e);
    preview->hide();
}

QRect VirtualWindow::calculatePreviewRect(const QPoint &pos) const
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

void VirtualWindow::dropEvent(QDropEvent *e)
{
    preview->hide();

    if (!e->mimeData()->hasFormat("application/x-widget-ptr"))
    {
        externalDrop(e->mimeData(), e->position().toPoint());
        e->acceptProposedAction();
        return;
    }

    QWidget *droppedWidget = nullptr;
    VirtualWindow *sourceWin = nullptr;
    QString title = e->mimeData()->text();

    if (!extractDragData(e->mimeData(), droppedWidget, sourceWin))
        return;

    QPoint pos = e->position().toPoint();
    int zone = determineDropZone(pos);

    if (sourceWin == this)
    {
        bool isFromList = (sourceWin->count() > 1);
        if (isFromList && zone == 0)
        {
            e->setDropAction(Qt::MoveAction);
            e->accept();
            return;
        }
        else if (!isFromList)
        {
            e->setDropAction(Qt::MoveAction);
            e->accept();
            return;
        }
    }

    int oldIdx = sourceWin->indexOf(droppedWidget);
    if (oldIdx != -1)
        sourceWin->removeTab(oldIdx);

    handleDrop(zone, droppedWidget, title);

    e->setDropAction(Qt::MoveAction);
    e->accept();
}

void VirtualWindow::externalDrop(const QMimeData *mime, const QPoint &pos)
{
    Q_UNUSED(mime);
    Q_UNUSED(pos);
}

int VirtualWindow::determineDropZone(const QPoint &pos) const
{
    int w = width();
    int h = height();

    if (pos.x() < w * 0.2)
        return 1;
    if (pos.x() > w * 0.8)
        return 2;
    if (pos.y() < h * 0.2)
        return 3;
    if (pos.y() > h * 0.8)
        return 4;

    return 0;
}

void VirtualWindow::handleDrop(int zone, QWidget *widget, const QString &title)
{
    switch (zone)
    {
    case 1:
        splitWindow(Qt::Horizontal, true, widget, title);
        break;
    case 2:
        splitWindow(Qt::Horizontal, false, widget, title);
        break;
    case 3:
        splitWindow(Qt::Vertical, true, widget, title);
        break;
    case 4:
        splitWindow(Qt::Vertical, false, widget, title);
        break;
    default:
        addTab(widget, title);
        break;
    }
}

void VirtualWindow::splitWindow(Qt::Orientation orientation, bool insertBefore, QWidget *widget, const QString &title)
{
    QSplitter *parentSplitter = qobject_cast<QSplitter *>(parentWidget());
    VirtualWindow *newWin = createNew();
    newWin->addTab(widget, title);

    if (!parentSplitter)
    {
        addTab(widget, title);
        return;
    }

    int idx = parentSplitter->indexOf(this);
    QList<int> parentSizes = parentSplitter->sizes();
    int currentSpace = parentSizes[idx];

    if (parentSplitter->orientation() == orientation)
    {
        parentSplitter->insertWidget(insertBefore ? idx : idx + 1, newWin);

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
            newSplitter->addWidget(newWin);
            newSplitter->addWidget(this);
        }
        else
        {
            newSplitter->addWidget(this);
            newSplitter->addWidget(newWin);
        }

        newSplitter->setSizes({currentSpace / 2, currentSpace / 2});
        parentSplitter->setSizes(parentSizes);
    }
}
