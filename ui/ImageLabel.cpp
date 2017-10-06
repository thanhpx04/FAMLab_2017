#include "ImageLabel.h"

ImageLabel::ImageLabel()
{

}

void ImageLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit sendMouseLocation(event->x(),event->y());
}
