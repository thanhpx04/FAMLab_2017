#include "SceneFragments.h"

SceneFragments::SceneFragments(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
}

