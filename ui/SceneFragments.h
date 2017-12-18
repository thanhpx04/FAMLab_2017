#ifndef SCENEFRAGMENTS_H
#define SCENEFRAGMENTS_H

#include <QGraphicsScene>
#include <QMenu>

class SceneFragments : public QGraphicsScene
{
    Q_OBJECT

public:
    SceneFragments(QMenu *itemMenu, QObject *parent = 0);

private:
    QMenu *myItemMenu;
};

#endif // SCENEFRAGMENTS_H
