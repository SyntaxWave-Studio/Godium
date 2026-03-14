#include "window_factory.h"
#include "editor_window.h"

#include <QString>
#include <QVariant>
#include <QFileInfo>

VirtualWindow* WindowFactory::createWindowFromUrl(const QUrl &url)
{
    if (!url.isLocalFile())
        return nullptr;

    QString path = url.toLocalFile();
    return createWindowFromPath(path);
}

VirtualWindow* WindowFactory::createWindowFromPath(const QString &path)
{
    QFileInfo fi(path);
    if (fi.exists() && fi.isFile())
    {
        EditorWindow *win = new EditorWindow();
        win->initializeContent(path);
        return win;
    }

    return nullptr;
}