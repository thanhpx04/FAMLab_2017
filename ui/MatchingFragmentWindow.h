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

public slots:
    void loadObject(ptrRGBAMatrix objectRGBAMatrix, vector<Point> border);

private:
    void createActions();
    void createFileMenuActions();
    void createFragmentMenuActions();
    void createHelpMenuActions();

    void createMenus();
    void createToolBars();

    MatchingFragmentScene *scene;
    QGraphicsView *view;

    // menu
    QMenu *fileMenu;
    QMenu *fragmentMenu;
    QMenu *helpMenu;

    // toolbar
    QToolBar *fileToolBar;
    QToolBar *fragmentToolBar;

    QAction *deleteAction;
    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *setLeftFragmentAction;
    QAction *setRightFragmentAction;

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
