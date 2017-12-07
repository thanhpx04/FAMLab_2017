
#include "FragmentItem.h"

FragmentItem::FragmentItem(Edge border, ptrRGBAMatrix objectRGBAMatrix, const QPixmap &pixmap, QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPixmapItem(pixmap, parent, scene)
{
    this->border = border;
    this->objectRGBAMatrix = objectRGBAMatrix;
    this->myContextMenu = contextMenu;
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsScenePositionChanges);
}

void FragmentItem::setBorder(Edge border)
{
    this->border = border;
}

Edge FragmentItem::getBorder()
{
    return this->border;
}

void FragmentItem::setObjectRGBAMatrix(ptrRGBAMatrix objectRGBAMatrix)
{
    this->objectRGBAMatrix = objectRGBAMatrix;
}

ptrRGBAMatrix FragmentItem::getObjectRGBAMatrix()
{
    return this->objectRGBAMatrix;
}

void FragmentItem::setListConnectedPoints(vector<Point> listConnectedPoints)
{
    this->listConnectedPoints = listConnectedPoints;
}

vector<Point> FragmentItem::getListConnectedPoints()
{
    return this->listConnectedPoints;
}

void FragmentItem::setconnectedLines(QVector<QGraphicsLineItem *> connectedLines, bool isFragment1)
{
    this->connectedLines = connectedLines;
    this->isFragment1 = isFragment1;
}

QVector<QGraphicsLineItem *> FragmentItem::getConnectedLines()
{
    return this->connectedLines;
}

void FragmentItem::updatePosition(QPointF newPosition)
{
    vector<Point> vectorPoints = this->getListConnectedPoints();
    for(size_t i=0;i<vectorPoints.size();i++)
    {
        // Converts the position (top-left) to the position of point
        Point pointI = vectorPoints.at(i);
        int xOffset = boundingRect().x() + pointI.getX();
        int yOffset = boundingRect().y() + pointI.getY();

        QPointF newPoint = QPointF(newPosition.x() + xOffset, newPosition.y() + yOffset);

        // Move the required point of the line
        QPointF p1 = isFragment1 ? newPoint : connectedLines.at(i)->line().p1();
        QPointF p2 = isFragment1 ? connectedLines.at(i)->line().p2() : newPoint;

        connectedLines.at(i)->setLine(QLineF(p1, p2));
    }
}

void FragmentItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant FragmentItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged)
    {
        // value is the new position.
        QPointF newPos = value.toPointF();

        updatePosition(newPos);
    }
    return QGraphicsItem::itemChange(change,value);
}


