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

#include "TestWindow.h"
#include "ui_TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    draftViewer = new DraftViewer;

    connect(this->draftViewer,SIGNAL(sendObjectRGBA(ptrRGBAMatrix)),this,SLOT(loadObject(ptrRGBAMatrix)));
}

TestWindow::~TestWindow()
{
    delete ui;
    delete scene;
    delete pixmapItem;
    delete draftViewer;
}

void TestWindow::loadImage(QString fn)
{
    qImage.load(fn);
    pixmapItem = scene->addPixmap(QPixmap::fromImage(qImage));
    pixmapItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

void TestWindow::mousePressEvent(QMouseEvent *mouseEvent)
{
//    QPointF pt = ui->graphicsView->mapToScene(mouseEvent->pos());
//    pixmapItem = scene->itemAt(pt);
    cout <<"aa"<<endl;
}

void TestWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::currentPath());
    cout << fileName.toStdString() << endl;
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        if (image.isNull())
        {
            QMessageBox::information(this, tr("MAELab"),
                tr("Cannot load %1.").arg(fileName));
            return;
        }


        draftViewer->loadImage(fileName);
        draftViewer->show();
    }
}

void TestWindow::on_btnRotateLeft_clicked()
{
    pixmapItem->rotate(-5);
}

void TestWindow::on_btnRotateRight_clicked()
{
    pixmapItem->rotate(+5);
}

void TestWindow::loadObject(ptrRGBAMatrix objectRGBAMatrix)
{
    cout << "received" << endl;
    qImage = ptrRGBAToQImage(objectRGBAMatrix);
    pixmapItem = scene->addPixmap(QPixmap::fromImage(qImage));
    pixmapItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}
