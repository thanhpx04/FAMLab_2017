#ifndef MATCHINGFRAGMENTWindow_H
#define MATCHINGFRAGMENTWindow_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QHBoxLayout>

#include "FragmentViewer.h"
#include "FragmentItem.h"
#include "MatchingFragmentScene.h"

class Image;


class MatchingFragmentWindow : public QMainWindow
{
    Q_OBJECT

public:
    MatchingFragmentWindow();
    ~MatchingFragmentWindow();

    QImage qImage;

    // rotate around center point with angle
    void rotateCenter(FragmentItem *selectedItem, int angle);

private slots:
    void open();
    void about();

    void deleteItem();
    void bringToFront();
    void sendToBack();
    void rotateleft();
    void rotateright();
    void setLeftFragment();
    void setRightFragment();
    void suggestDTW();

public slots:
    void loadObject(ptrRGBAMatrix objectRGBAMatrix, Edge border);

private:
    void createActions();
    void createFileMenuActions();
    void createFragmentMenuActions();
    void createMatchingMenuActions();
    void createHelpMenuActions();

    void createMenus();
    void createToolBars();

    vector<Point> findMinXmappingY(Edge edge);
    vector<Point> findMaxXmappingY(Edge edge);
    vector<Point> getSubPointsByDistance(vector<Point> listPoints, int distance);
    vector<int> getXvalueOfListPoints(vector<Point> listPoints);

    MatchingFragmentScene *scene;
    QGraphicsView *view;

    // menu
    QMenu *fileMenu;
    QMenu *fragmentMenu;
    QMenu *matchingMenu;
    QMenu *helpMenu;

    // toolbar
    QToolBar *fileToolBar;
    QToolBar *fragmentToolBar;
    QToolBar *matchingToolBar;

    QAction *deleteAction;
    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *setLeftFragmentAction;
    QAction *setRightFragmentAction;
    QAction *suggestDTWAction;

    // actions
    QAction *openAction;
    QAction *exitAction;
    QAction *aboutAction;

    // fragment items left and right
    FragmentItem *leftFragment;
    FragmentItem *rightFragment;

    // image viewer
    FragmentViewer *fragmentViewer;

    FragmentItem *selectedFragmentItem() const;


};

#endif // MATCHINGFRAGMENTWindow_H
