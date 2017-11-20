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
#include "../io/Reader.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"
#include "../segmentation/Suzuki.h"
#include "../segmentation/Projection.h"
#include "../segmentation/Filters.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "../utils/ImageConvert.h"

#include "MatchingFragmentWindow.h"
#include "ui_MatchingFragmentWindow.h"

MatchingFragmentWindow::MatchingFragmentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MatchingFragmentWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    scene = new MatchingFragmentScene(itemMenu);
    ui->graphicsView->setScene(scene);

    fragmentViewer = new FragmentViewer;

    setWindowIcon(QIcon("./resources/ico/FAMLab.png"));

    connect(this->fragmentViewer,SIGNAL(sendObjectRGBA(ptrRGBAMatrix,vector<Edge>)),this,SLOT(loadObject(ptrRGBAMatrix,vector<Edge>)));
    createActions();
}

MatchingFragmentWindow::~MatchingFragmentWindow()
{
    delete ui;
    delete scene;
    delete itemMenu;

    delete deleteAction;
    delete toFrontAction;
    delete sendBackAction;

    delete fragmentViewer;

}

void MatchingFragmentWindow::rotateCenter(QGraphicsItem* selectedItem, int angle)
{
    QRectF rect;
    rect = selectedItem->mapToScene(selectedItem->boundingRect()).boundingRect();
    QPointF center = rect.center();
    QTransform transform;
    transform.translate(center.x(), center.y());
    transform.rotate(angle);
    transform.translate(-center.x(), -center.y());
    selectedItem->setPos(transform.map(selectedItem->pos()));
    selectedItem->setRotation(selectedItem->rotation() + angle);
}

FragmentItem *MatchingFragmentWindow::selectedFragmentItem() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if(items.count()==1)
        return dynamic_cast<FragmentItem *> (items.first());
    else
        return 0;
}

void MatchingFragmentWindow::createActions()
{
    deleteAction = new QAction(QIcon("./resources/ico/delete.png"),
                               tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from screen"));
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteItem()));

    toFrontAction = new QAction(QIcon("./resources/ico/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon("./resources/ico/sendtoback.png"),
                                 tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));
}

void MatchingFragmentWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    cout << fileName.toStdString() << endl;
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        if (image.isNull())
        {
            QMessageBox::information(this, tr("Papyrus FEMLab"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        fragmentViewer->loadImage(fileName);
        fragmentViewer->show();
    }
}

void MatchingFragmentWindow::on_btnRotateLeft_clicked()
{
//    fragment1 = selectedFragmentItem();
//    if(fragment1)
//        rotateCenter(fragment1,-1);
}

void MatchingFragmentWindow::on_btnRotateRight_clicked()
{
//    fragment1 = selectedFragmentItem();
//    if(fragment1)
    //        rotateCenter(fragment1,+1);
}

void MatchingFragmentWindow::deleteItem()
{
    FragmentItem *selected = selectedFragmentItem();
    if(selected){
        scene->removeItem(selected);
    }
}

void MatchingFragmentWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MatchingFragmentWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MatchingFragmentWindow::loadObject(ptrRGBAMatrix objectRGBAMatrix, vector<Edge> listOfEdges)
{
    cout << "received" << endl;
    qImage = ptrRGBAToQImage(objectRGBAMatrix);

    FragmentItem *pixmapItem = new FragmentItem(listOfEdges, QPixmap::fromImage(qImage), itemMenu);

    scene->addItem(pixmapItem);
}

void MatchingFragmentWindow::createMenus()
{
    itemMenu = menuBar()->addMenu(tr("&Fragment"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);
}
