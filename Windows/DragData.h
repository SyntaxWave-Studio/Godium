#ifndef DRAGDATA_H
#define DRAGDATA_H

#include <QWidget>
#include <VirtualWindow.h>

struct DragData
{
    QWidget *widget = nullptr;
    VirtualWindow *sourceWindow = nullptr;
    QString title;

    QByteArray serialize() const
    {
        QByteArray data;
        QDataStream ds(&data, QIODevice::WriteOnly);
        ds << reinterpret_cast<quintptr>(widget)
           << reinterpret_cast<quintptr>(sourceWindow)
           << title;
        return data;
    }

    static DragData deserialize(const QMimeData *mime)
    {
        DragData dd;
        QByteArray data = mime->data("application/x-widget-ptr");
        QDataStream ds(&data, QIODevice::ReadOnly);
        quintptr wPtr = 0, swPtr = 0;
        ds >> wPtr >> swPtr >> dd.title;
        dd.widget = reinterpret_cast<QWidget *>(wPtr);
        dd.sourceWindow = reinterpret_cast<VirtualWindow *>(swPtr);
        return dd;
    }
};

#endif
