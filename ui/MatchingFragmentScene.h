#ifndef MATCHINGFRAGMENTSCENE_H
#define MATCHINGFRAGMENTSCENE_H

#include <QGraphicsScene>
#include <QMenu>

class MatchingFragmentScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MatchingFragmentScene(QMenu *itemMenu, QObject *parent = 0);

private:
    QMenu *myItemMenu;
};

#endif // MATCHINGFRAGMENTSCENE_H
