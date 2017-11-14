#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>

class Link : public QGraphicsLineItem
{
public:
    Link(QPointF fromPoint, QPointF toPoint);
    ~Link();

    QPointF fromPoint() const;
    QPointF toPoint() const;

    void setColor(const QColor &color);
    QColor color() const;

    // update moving Fragments
    void trackFragments();

private:
    QPointF theFromPoint;
    QPointF theToPoint;
};

#endif // LINK_H
