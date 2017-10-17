#ifndef TESTWINDOW_H
#define TESTWINDOW_H



#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

#include "DraftViewer.h"




class Image;

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();

    QImage qImage;

    void loadImage(QString fn);
    void mousePressEvent(QMouseEvent *mouseEvent);

private slots:
    void on_actionOpen_triggered();
    void on_btnRotateLeft_clicked();
    void on_btnRotateRight_clicked();

public slots:
    void loadObject(ptrRGBAMatrix objectRGBAMatrix);

private:
    Ui::TestWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;

    DraftViewer *draftViewer;

};

#endif // TESTWINDOW_H
