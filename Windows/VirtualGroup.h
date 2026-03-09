#ifndef VIRTUALGROUP_H
#define VIRTUALGROUP_H

#include "VirtualWindow.h"
#include <QTabWidget>
#include <QSplitter>
#include <QDrag>
#include <QMimeData>
#include <QPointer>
#include <QTimer>
#include <QRubberBand>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include <QTabBar>

enum DropZone
{
    ZoneCenter = 0,
    ZoneLeft = 1,
    ZoneRight = 2,
    ZoneTop = 3,
    ZoneBottom = 4
};

class VirtualGroup : public QTabWidget
{
    Q_OBJECT
public:
    explicit VirtualGroup(QWidget *parent = nullptr);
    void addWindow(VirtualWindow *window, const QString &title);

private:
    QPoint dragStartPos;
    QRubberBand *preview;

    void setupUi();
    void setupStyle();
    void setupPreview();

    void handleClose(int index);
    bool eventFilter(QObject *obj, QEvent *e) override;

    void startDrag(int idx);
    void createFloatingWindow(VirtualWindow *window, const QString &title);

    void checkEmptyAndCleanup();
    void cleanupStructure(QSplitter *splitter);

    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dropEvent(QDropEvent *e) override;

    QRect calculatePreviewRect(const QPoint &pos) const;
    int determineDropZone(const QPoint &pos) const;

    void handleDrop(int zone, VirtualWindow *window, const QString &title);
    void splitWindow(Qt::Orientation orientation, bool insertBefore, VirtualWindow *window, const QString &title);
};

#endif
