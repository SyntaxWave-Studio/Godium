#ifndef WINDOW_FACTORY_H
#define WINDOW_FACTORY_H

#include "virtual_window.h"

class WindowFactory
{
public:
    static VirtualWindow *createWindowFromUrl(const QUrl &url);
    static VirtualWindow *createWindowFromPath(const QString &path);
};

#endif
