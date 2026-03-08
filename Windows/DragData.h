#ifndef DRAGDATA_H
#define DRAGDATA_H

#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QString>
#include <QIODevice>

class VirtualWindow;

struct DragData
{
    VirtualWindow *window = nullptr;
    QString title;

    QByteArray serialize() const
    {
        QByteArray data;
        QDataStream ds(&data, QIODevice::WriteOnly);
        ds << reinterpret_cast<quintptr>(window)
           << title;
        return data;
    }

    static DragData deserialize(const QMimeData *mime)
    {
        DragData dd;
        QByteArray data = mime->data("application/x-virtualwindow-ptr");
        QDataStream ds(&data, QIODevice::ReadOnly);
        quintptr wPtr = 0;
        ds >> wPtr >> dd.title;
        dd.window = reinterpret_cast<VirtualWindow *>(wPtr);
        return dd;
    }
};

#endif
