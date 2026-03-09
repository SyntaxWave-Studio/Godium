#ifndef WINDOWFACTORY_H
#define WINDOWFACTORY_H

#include "editor_window.h"

#include <QString>
#include <QVariant>
#include <QFileInfo>

class VirtualWindow;

class WindowFactory
{
public:
    static VirtualWindow *createWindowFromUrl(const QUrl &url)
    {
        if (!url.isLocalFile())
            return nullptr;

        QString path = url.toLocalFile();
        return createWindowFromPath(path);
    }

    static VirtualWindow *createWindowFromPath(const QString &path)
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
};

#endif
