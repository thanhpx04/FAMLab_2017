#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QMouseEvent>


class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    ImageLabel();

    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void sendMouseLocation(int x, int y);
};

#endif // IMAGELABEL_H
