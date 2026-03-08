#ifndef VIRTUALWINDOW_H
#define VIRTUALWINDOW_H

#include <QTabWidget>
#include <QPointer>
#include <QTabBar>
#include <QRubberBand>
#include <QSplitter>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>

class VirtualWindow : public QTabWidget
{
    Q_OBJECT
public:
    explicit VirtualWindow(QWidget *parent = nullptr);
    virtual ~VirtualWindow() = default;

    virtual VirtualWindow *createNew() = 0;
    virtual void initializeContent(const QVariant &data) = 0;

private:
    void setupUi();
    void setupStyle();
    void setupPreview();

    void handleClose(int index);
    bool eventFilter(QObject *obj, QEvent *e);
    void createFloatingWindow(VirtualWindow *page, const QString &title);

    void checkEmptyAndCleanup();
    void cleanupStructure(QSplitter *splitter);

    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void startDrag(int idx);

    void dropEvent(QDropEvent *e);
    void externalDrop(const QMimeData *mime, const QPoint &pos);

    QRect calculatePreviewRect(const QPoint &pos) const;
    int determineDropZone(const QPoint &pos) const;

    void handleDrop(int zone, VirtualWindow *window, const QString &title);
    void splitWindow(Qt::Orientation orientation, bool insertBefore, VirtualWindow *window, const QString &title);

    QRubberBand *preview = nullptr;
    QPoint dragStartPos;
};

#endif
