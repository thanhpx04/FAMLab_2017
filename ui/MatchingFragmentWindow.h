#ifndef MATCHINGFRAGMENTWindow_H
#define MATCHINGFRAGMENTWindow_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QAction>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#include "FragmentViewer.h"
#include "FragmentItem.h"
#include "MatchingFragmentScene.h"

class Image;

namespace Ui {
class MatchingFragmentWindow;
}

class MatchingFragmentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MatchingFragmentWindow(QWidget *parent = 0);
    ~MatchingFragmentWindow();

    QImage qImage;

//    void mousePressEvent(QMouseEvent *mouseEvent);
//    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    // rotate around center point with angle
    void rotateCenter(QGraphicsItem* selectedItem, int angle);


private slots:
    void on_actionOpen_triggered();
    void on_btnRotateLeft_clicked();
    void on_btnRotateRight_clicked();
    void deleteItem();
    void bringToFront();
    void sendToBack();


public slots:
    void loadObject(ptrRGBAMatrix objectRGBAMatrix, vector<Edge> listOfEdges);

private:
    void createActions();
    void createMenus();

    Ui::MatchingFragmentWindow *ui;
    MatchingFragmentScene *scene;

    QMenu *itemMenu;

    QAction *deleteAction;
    QAction *toFrontAction;
    QAction *sendBackAction;


    FragmentViewer *fragmentViewer;

    FragmentItem *selectedFragmentItem() const;


};

#endif // MATCHINGFRAGMENTWindow_H
