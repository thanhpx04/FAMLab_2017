
#include "FragmentItem.h"

FragmentItem::FragmentItem(vector<Edge> listOfEdges, const QPixmap &pixmap, QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPixmapItem(pixmap, parent, scene)
{
    this->listOfEdges = listOfEdges;
    this->myContextMenu = contextMenu;
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

void FragmentItem::setListOfEdges(vector<Edge> listOfEdges)
{
    this->listOfEdges = listOfEdges;
}

vector<Edge> FragmentItem::getListOfEdges()
{
    return this->listOfEdges;
}

//void FragmentItem::setListConnectedPoints(QVector<QPointF> listConnectedPoints)
//{
//    this->listConnectedPoints = listConnectedPoints;
//}

//QVector<QPointF> FragmentItem::getListConnectedPoints()
//{
//    return this->listConnectedPoints;
//}

//void FragmentItem::setconnectedLines(QVector<QGraphicsLineItem *> connectedLines, bool isFragment1)
//{
//    this->connectedLines = connectedLines;
//    this->isFragment1 = isFragment1;
//}

//QVector<QGraphicsLineItem *> FragmentItem::getConnectedLines()
//{
//    return this->connectedLines;
//}

//void FragmentItem::updatePosition(QPointF newPosition)
//{
//    QVector<QPointF> vectorPoints = this->getListConnectedPoints();
//    for(int i=0;i<vectorPoints.size();i++)
//    {
//        // Converts the position (top-left) to the position of point
//        int xOffset = boundingRect().x() + vectorPoints.at(i).x();
//        int yOffset = boundingRect().y() + vectorPoints.at(i).y();

//        QPointF newPoint = QPointF(newPosition.x() + xOffset, newPosition.y() + yOffset);

//        // Move the required point of the line
//        QPointF p1 = isFragment1 ? newPoint : connectedLines.at(i)->line().p1();
//        QPointF p2 = isFragment1 ? connectedLines.at(i)->line().p2() : newPoint;

//        connectedLines.at(i)->setLine(QLineF(p1, p2));
//    }
//}

void FragmentItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

//QVariant FragmentItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
//{
//    if(change == ItemPositionHasChanged)
//    {
//        // value is the new position.
//        QPointF newPos = value.toPointF();

//        updatePosition(newPos);
//    }
//    return QGraphicsItem::itemChange(change,value);
//}


