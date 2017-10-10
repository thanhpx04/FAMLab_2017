#include <iostream>
using namespace std;

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "TestWindow.h"
#include "ui_TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);


}

TestWindow::~TestWindow()
{
    delete ui;
    delete scene;
    delete pixmapItem;
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

        this->loadImage(fileName);
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
