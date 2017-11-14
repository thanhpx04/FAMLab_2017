#ifndef MATCHINGFRAGMENTWindow_H
#define MATCHINGFRAGMENTWindow_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#include "FragmentViewer.h"
#include "FragmentItem.h"

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

//    void loadImage(QString fn);
//    void mousePressEvent(QMouseEvent *mouseEvent);
//    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    // rotate around center point with angle
    void rotateCenter(QGraphicsItem* selectedItem, int angle);

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    void on_actionOpen_triggered();
    void on_btnRotateLeft_clicked();
    void on_btnRotateRight_clicked();

    void on_pushButton_clicked();

    void chooseFragment1();
    void chooseFragment2();

public slots:
    void loadObject(ptrRGBAMatrix objectRGBAMatrix, vector<Edge> listOfEdges);

private:
    Ui::MatchingFragmentWindow *ui;
    QGraphicsScene *scene;
    FragmentItem *fragment1;
    FragmentItem *fragment2;
    FragmentViewer *fragmentViewer;

    FragmentItem *selectedFragmentItem() const;

    void createActions();
//    void createMenus();

    QAction *chooseFragment1Act;
    QAction *chooseFragment2Act;

};

#endif // MATCHINGFRAGMENTWindow_H
