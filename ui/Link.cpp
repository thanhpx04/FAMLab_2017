#include <QtCore>
#include <QPen>
#include "Link.h"

Link::Link(QPointF fromPoint, QPointF toPoint)
{
    theFromPoint = fromPoint;
    theToPoint = toPoint;

//    theFromFragment->addLink(this);
//    theToFragment->addLink(this);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    setColor(Qt::red);
    trackFragments();
}

Link::~Link()
{
//    theFromFragment->removeLink(this);
//    theToFragment->removeLink(this);
}

void Link::setColor(const QColor &color)
{
        setPen(QPen(color, 1));
}

void Link::trackFragments()
{
    //set each line between Fragments
}
