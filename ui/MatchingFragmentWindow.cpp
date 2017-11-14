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

    scene = new QGraphicsScene(this);
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
    delete fragment1;
    delete fragment2;
    delete fragmentViewer;
    delete chooseFragment1Act;
    delete chooseFragment2Act;
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

#ifndef QT_NO_CONTEXTMENU
void MatchingFragmentWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(chooseFragment1Act);
    menu.addAction(chooseFragment2Act);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

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
    chooseFragment1Act = new QAction(tr("&Set 1st Fragment"), this);
//    chooseFragment1Act->setShortcuts(QKeySequence::);
    chooseFragment1Act->setStatusTip(tr("Choose the first Fragment"));
    connect(chooseFragment1Act, SIGNAL(triggered()), this, SLOT(chooseFragment1()));

    chooseFragment2Act = new QAction(tr("&Set 2nd Fragment"), this);
//    chooseFragment2Act->setShortcuts(QKeySequence::);
    chooseFragment2Act->setStatusTip(tr("Choose second Fragment"));
    connect(chooseFragment2Act, SIGNAL(triggered()), this, SLOT(chooseFragment2()));
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
    fragment1 = selectedFragmentItem();
    if(fragment1)
        rotateCenter(fragment1,-1);
}

void MatchingFragmentWindow::on_btnRotateRight_clicked()
{
    fragment1 = selectedFragmentItem();
    if(fragment1)
        rotateCenter(fragment1,+1);
}

void MatchingFragmentWindow::loadObject(ptrRGBAMatrix objectRGBAMatrix, vector<Edge> listOfEdges)
{
    cout << "received" << endl;
    qImage = ptrRGBAToQImage(objectRGBAMatrix);

    FragmentItem *pixmapItem = new FragmentItem(listOfEdges, QPixmap::fromImage(qImage));

    scene->addItem(pixmapItem);
}

void MatchingFragmentWindow::chooseFragment2()
{
    fragment2 = selectedFragmentItem();
    if(fragment2)
        cout<<"2"<<endl;
}

void MatchingFragmentWindow::chooseFragment1()
{
    fragment1 = selectedFragmentItem();
    if(fragment1)
        cout<<"1"<<endl;
}
