#include "VirtualWindow.h"
#include "MainWindow.h"
#include "WindowFactory.h"
#include "DragData.h"

enum DropZone
{
    ZoneCenter = 0,
    ZoneLeft = 1,
    ZoneRight = 2,
    ZoneTop = 3,
    ZoneBottom = 4
};

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

void VirtualWindow::createFloatingWindow(VirtualWindow *window, const QString &title)
{
    if (!window)
        return;

    QWidget *oldParent = window->parentWidget();
    if (oldParent)
    {
        QSplitter *oldSplitter = qobject_cast<QSplitter *>(oldParent);
        if (oldSplitter)
        {
            window->setParent(nullptr);
            cleanupStructure(oldSplitter);
        }
        else
        {
            window->setParent(nullptr);
        }
    }

    MainWindow *floatingMainWin = new MainWindow();
    floatingMainWin->splitter()->addWidget(window);
    window->setParent(floatingMainWin->splitter());

    int baseSize = this->height();
    int side = static_cast<int>(baseSize * 0.85);
    side = std::clamp(side, 500, 1200);

    floatingMainWin->resize(side, side);
    floatingMainWin->move(QCursor::pos() - QPoint(side / 2, 30));

    floatingMainWin->setWindowTitle(title);
    floatingMainWin->show();

    checkEmptyAndCleanup();
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

    QPointer<QWidget> safeTopLevel = topLevel;
    QTimer::singleShot(0, [safeTopLevel]()
                       {
        if (!safeTopLevel)
            return;

        QList<VirtualWindow *> remainingWindows = safeTopLevel->findChildren<VirtualWindow *>();

        if (remainingWindows.isEmpty()) {
            safeTopLevel->close();
        } });
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

void VirtualWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasFormat("application/x-virtualwindow-ptr") || e->mimeData()->hasUrls())
        e->acceptProposedAction();
    else
        e->ignore();
}

void VirtualWindow::dragMoveEvent(QDragMoveEvent *e)
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

void VirtualWindow::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e);
    preview->hide();
}

void VirtualWindow::startDrag(int idx)
{
    if (idx < 0 || idx >= count())
        return;

    DragData dd;
    dd.window = this;
    dd.title = windowTitle();

    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData();

    mime->setData("application/x-virtualwindow-ptr", dd.serialize());
    mime->setText(dd.title);

    drag->setMimeData(mime);

    Qt::DropAction result = drag->exec(Qt::MoveAction);
    if (result == Qt::IgnoreAction)
    {
        createFloatingWindow(this, dd.title);
    }

    checkEmptyAndCleanup();
}

void VirtualWindow::dropEvent(QDropEvent *e)
{
    preview->hide();

    if (e->mimeData()->hasFormat("application/x-virtualwindow-ptr"))
    {
        DragData dd = DragData::deserialize(e->mimeData());

        VirtualWindow *droppedWindow = dd.window;
        if (!droppedWindow)
        {
            e->ignore();
            return;
        }

        if (droppedWindow == this)
        {
            e->setDropAction(Qt::MoveAction);
            e->accept();
            return;
        }

        QWidget *oldParent = droppedWindow->parentWidget();
        if (oldParent)
        {
            QSplitter *oldSplitter = qobject_cast<QSplitter *>(oldParent);
            if (oldSplitter)
            {
                droppedWindow->setParent(nullptr);
                cleanupStructure(oldSplitter);
            }
            else
            {
                droppedWindow->setParent(nullptr);
            }
        }

        int zone = determineDropZone(e->position().toPoint());
        handleDrop(zone, droppedWindow, dd.title);

        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else if (e->mimeData()->hasUrls())
    {
        externalDrop(e->mimeData(), e->position().toPoint());
        e->setDropAction(Qt::CopyAction);
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void VirtualWindow::externalDrop(const QMimeData *mime, const QPoint &pos)
{
    if (mime->hasUrls())
    {
        QList<QUrl> urls = mime->urls();
        for (const QUrl &url : urls)
        {
            QString localPath = url.toLocalFile();
            if (!localPath.isEmpty())
            {
                VirtualWindow *newWin = WindowFactory::createWindow(localPath);
                if (!newWin)
                {
                    qWarning() << "WindowFactory не удалось создать окно для пути:" << localPath;
                    continue;
                }

                int zone = determineDropZone(pos);
                handleDrop(zone, newWin, newWin->windowTitle());
            }
        }
    }
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

int VirtualWindow::determineDropZone(const QPoint &pos) const
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

void VirtualWindow::handleDrop(int zone, VirtualWindow *window, const QString &title)
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
        addTab(window, title);
        break;
    }
}

void VirtualWindow::splitWindow(Qt::Orientation orientation, bool insertBefore, VirtualWindow *window, const QString &title)
{
    QSplitter *parentSplitter = qobject_cast<QSplitter *>(parentWidget());
    if (!parentSplitter)
    {
        addTab(window, title);
        return;
    }

    int idx = parentSplitter->indexOf(this);
    QList<int> parentSizes = parentSplitter->sizes();
    int currentSpace = parentSizes[idx];

    if (parentSplitter->orientation() == orientation)
    {
        parentSplitter->insertWidget(insertBefore ? idx : idx + 1, window);

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
            newSplitter->addWidget(window);
            newSplitter->addWidget(this);
        }
        else
        {
            newSplitter->addWidget(this);
            newSplitter->addWidget(window);
        }

        newSplitter->setSizes({currentSpace / 2, currentSpace / 2});
        parentSplitter->setSizes(parentSizes);
    }
}
