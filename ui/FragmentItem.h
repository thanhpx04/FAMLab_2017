#ifndef FRAGMENTITEM_H
#define FRAGMENTITEM_H
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <float.h>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include <QtCore>
#include <QGraphicsScene>
#include <QMenu>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneContextMenuEvent>

class FragmentItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    FragmentItem(Edge border, ptrRGBAMatrix objectRGBAMatrix, const QPixmap &pixmap, QMenu *contextMenu, QGraphicsItem *parent = 0,
                 QGraphicsScene *scene = 0);
    void setBorder(Edge border);
    Edge getBorder();
    void setObjectRGBAMatrix(ptrRGBAMatrix objectRGBAMatrix);
    ptrRGBAMatrix getObjectRGBAMatrix();

//    void setListConnectedPoints(QVector<QPointF> listConnectedPoints);
//    QVector<QPointF> getListConnectedPoints();
//    void setconnectedLines(QVector<QGraphicsLineItem*> connectedLines, bool isFragment1);
//    QVector<QGraphicsLineItem*> getConnectedLines();

    void updatePosition(QPointF newPosition);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
//    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    Edge border;
    ptrRGBAMatrix objectRGBAMatrix;
    QMenu *myContextMenu;
    // attributes for drawing connected lines between Fragments
//    QVector<QPointF> listConnectedPoints;
//    QVector<QGraphicsLineItem*> connectedLines;
//    bool isFragment1;
    //----------------------------------------

};

#endif // FRAGMENTITEM_H
