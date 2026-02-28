#include "VirtualWindow.h"
#include "MainWindow.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QApplication>
#include <QTimer>

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
        "QTabBar::close-button { subcontrol-origin: margin; subcontrol-position: right; margin-right: 4px; width: 16px; }"
        "QTabBar::close-button:hover { background: #454545; border-radius: 2px; }"
    );
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

QMimeData *VirtualWindow::createMimeData(QWidget *page, const QString &title)
{
    QMimeData *mime = new QMimeData;
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    ds << reinterpret_cast<quintptr>(page) << reinterpret_cast<quintptr>(this);
    mime->setData("application/x-widget-ptr", data);
    mime->setText(title);
    return mime;
}

void VirtualWindow::createFloatingWindow(QWidget *page, const QString &title)
{
    int realIdx = indexOf(page);
    if (realIdx != -1)
        removeTab(realIdx);

    MainWindow *floatingWin = new MainWindow();
    floatingWin->resize(this->size());
    floatingWin->move(QCursor::pos() - QPoint(50, 10));

    VirtualWindow *newWin = createNew();
    newWin->addTab(page, title);
    floatingWin->splitter()->addWidget(newWin);
    floatingWin->show();
}

void VirtualWindow::checkEmptyAndCleanup()
{
    if (count() == 0)
    {
        QWidget *topLevel = window();
        QSplitter *parentSplitter = qobject_cast<QSplitter *>(parentWidget());

        QTimer::singleShot(0, [this, parentSplitter, topLevel]() {
            if (parentSplitter)
                cleanupStructure(parentSplitter);
            this->deleteLater();

            if (topLevel && qobject_cast<QMainWindow *>(topLevel) && topLevel->findChildren<VirtualWindow *>().count() <= 1)
                topLevel->close(); 
        });
    }
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
        else if (topLevel && qobject_cast<QMainWindow *>(topLevel) && QApplication::topLevelWidgets().count() > 1)
            topLevel->close();
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

    if (sourceWin == this && zone == 0)
    {
        e->setDropAction(Qt::LinkAction);
        e->accept();
        return;
    }

    int oldIdx = sourceWin->indexOf(droppedWidget);
    if (oldIdx != -1)
        sourceWin->removeTab(oldIdx);

    handleDrop(zone, droppedWidget, title);
    e->acceptProposedAction();
}

void VirtualWindow::externalDrop(const QMimeData *mime, const QPoint &pos)
{
    Q_UNUSED(mime);
    Q_UNUSED(pos);
}

bool VirtualWindow::extractDragData(const QMimeData *mime, QWidget *&widget, VirtualWindow *&sourceWin)
{
    QByteArray data = mime->data("application/x-widget-ptr");
    QDataStream ds(&data, QIODevice::ReadOnly);
    quintptr pagePtr = 0, sourceWinPtr = 0;
    ds >> pagePtr >> sourceWinPtr;

    widget = reinterpret_cast<QWidget *>(pagePtr);
    sourceWin = reinterpret_cast<VirtualWindow *>(sourceWinPtr);

    return widget && sourceWin;
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

    if (parentSplitter->orientation() == orientation)
    {
        parentSplitter->insertWidget(insertBefore ? idx : idx + 1, newWin);

        QList<int> sizes = parentSplitter->sizes();
        int totalSpace = sizes[idx];
        sizes[idx] = totalSpace / 2;
        sizes.insert(insertBefore ? idx : idx + 1, totalSpace / 2);
        parentSplitter->setSizes(sizes);
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

        int currentSize = (orientation == Qt::Horizontal) ? width() : height();
        newSplitter->setSizes({currentSize / 2, currentSize / 2});
    }
}
