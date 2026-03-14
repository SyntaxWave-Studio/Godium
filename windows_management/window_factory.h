#ifndef WINDOWFACTORY_H
#define WINDOWFACTORY_H

class VirtualWindow;

class WindowFactory
{
public:
    static VirtualWindow *createWindowFromUrl(const QUrl &url);
    static VirtualWindow *createWindowFromPath(const QString &path);
};

#endif
