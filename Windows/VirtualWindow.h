#ifndef VIRTUALWINDOW_H
#define VIRTUALWINDOW_H

#include <QTabWidget>
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

    virtual void initializeContent(const QVariant &data) = 0;
    virtual VirtualWindow *createNew() = 0;
private:
    void setupUi();
    void setupStyle();
    void setupPreview();

    void handleClose(int index);
    bool eventFilter(QObject *obj, QEvent *e) override;

    void cleanupStructure(QSplitter *splitter);
    QMimeData *createMimeData(QWidget *page, const QString &title);
    void createFloatingWindow(QWidget *page, const QString &title);
    void checkEmptyAndCleanup();

    QRect calculatePreviewRect(const QPoint &pos) const;
    bool extractDragData(const QMimeData *mime, QWidget *&widget, VirtualWindow *&sourceWin);
    int determineDropZone(const QPoint &pos) const;

    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;

    void startDrag(int idx);
    void dropEvent(QDropEvent *e) override;

    void handleDrop(int zone, QWidget *widget, const QString &title);
    void splitWindow(Qt::Orientation orientation, bool insertBefore, QWidget *widget, const QString &title);

    private : QRubberBand *preview = nullptr;
    QPoint dragStartPos;
};

#endif
