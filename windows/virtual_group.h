#ifndef VIRTUALGROUP_H
#define VIRTUALGROUP_H

#include "virtual_window.h"

#include <QTabWidget>
#include <QPoint>
#include <QRect>
#include <QSplitter>
#include <QRubberBand>

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

    VirtualGroup* handleDrop(int zone, VirtualWindow *window, const QString &title);
    VirtualGroup* splitWindow(Qt::Orientation orientation, bool insertBefore, VirtualWindow *window, const QString &title);
};

#endif
