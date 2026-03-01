#ifndef WINDOWFACTORY_H
#define WINDOWFACTORY_H

#include "EditorWindow.h"
#include <QString>
#include <QVariant>
#include <QFileInfo>

class VirtualWindow;

class WindowFactory
{
public:
    static VirtualWindow *createWindow(const QVariant &data)
    {
        QString path = data.toString();
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
