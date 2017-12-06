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
#include "algorithm/MatchingAlgorithm.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "../utils/ImageConvert.h"

#include "MatchingFragmentWindow.h"

MatchingFragmentWindow::MatchingFragmentWindow()
{
    createActions();
    createMenus();
    createToolBars();

    scene = new MatchingFragmentScene(fragmentMenu);

    fragmentViewer = new FragmentViewer;

    setWindowIcon(QIcon(":/Icons/resources/ico/FAMLab.png"));

    connect(this->fragmentViewer,SIGNAL(sendObjectRGBA(ptrRGBAMatrix,Edge)),this,SLOT(loadObject(ptrRGBAMatrix,Edge)));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    view = new QGraphicsView(scene);
    mainLayout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);

    setCentralWidget(widget);
    setWindowTitle(tr(".: Papyrus FAMLab :."));
    setUnifiedTitleAndToolBarOnMac(true);
}

MatchingFragmentWindow::~MatchingFragmentWindow()
{
    delete scene;
    delete view;

    delete fileMenu;
    delete fragmentMenu;
    delete matchingMenu;
    delete helpMenu;

    delete fileToolBar;
    delete fragmentToolBar;
    delete matchingToolBar;

    delete deleteAction;
    delete toFrontAction;
    delete sendBackAction;
    delete rotateLeftAction;
    delete rotateRightAction;
    delete setLeftFragmentAction;
    delete setRightFragmentAction;
    delete suggestDTWAction;

    delete openAction;
    delete exitAction;
    delete aboutAction;

    delete leftFragment;
    delete rightFragment;

    delete fragmentViewer;
}

void MatchingFragmentWindow::rotateCenter(FragmentItem* selectedItem, int angle)
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

void MatchingFragmentWindow::open()
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

void MatchingFragmentWindow::about()
{
    QMessageBox::about(this, tr("About FAMLab"),
                       tr(
                           "<p><b>FAMLab</b> is a software in computer vision. It provides the functions to "
                           "extraction and matching fragments.</p>"));
}

FragmentItem *MatchingFragmentWindow::selectedFragmentItem() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if(items.count()==1)
        return dynamic_cast<FragmentItem *> (items.first());
    else
        return 0;
}

void MatchingFragmentWindow::createFragmentMenuActions()
{
    deleteAction = new QAction(QIcon(":/Icons/resources/ico/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from screen"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    toFrontAction = new QAction(QIcon(":/Icons/resources/ico/bringtofront.png"), tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/Icons/resources/ico/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    rotateLeftAction = new QAction(QIcon(":/Icons/resources/ico/rotateleft.png"), tr("Rotate &Left"), this);
    rotateLeftAction->setShortcut(tr("Ctrl+L"));
    rotateLeftAction->setStatusTip(tr("Rotate left"));
    connect(rotateLeftAction, SIGNAL(triggered()), this, SLOT(rotateleft()));

    rotateRightAction = new QAction(QIcon(":/Icons/resources/ico/rotateright.png"), tr("Rotate &Right"), this);
    rotateRightAction->setShortcut(tr("Ctrl+R"));
    rotateRightAction->setStatusTip(tr("Rotate Right"));
    connect(rotateRightAction, SIGNAL(triggered()), this, SLOT(rotateright()));

    setLeftFragmentAction = new QAction(QIcon(":/Icons/resources/ico/rotateright.png"), tr("Set the left fragment"), this);
    setLeftFragmentAction->setStatusTip(tr("Set the left fragment"));
    connect(setLeftFragmentAction, SIGNAL(triggered()), this, SLOT(setLeftFragment()));

    setRightFragmentAction = new QAction(QIcon(":/Icons/resources/ico/rotateright.png"), tr("Set the right fragment"), this);
    setRightFragmentAction->setStatusTip(tr("Set the right fragment"));
    connect(setRightFragmentAction, SIGNAL(triggered()), this, SLOT(setRightFragment()));
}

void MatchingFragmentWindow::createMatchingMenuActions()
{
    suggestDTWAction = new QAction(QIcon(":/Icons/resources/ico/rotateright.png"), tr("suggest matching by DTW"), this);
    //    suggestDTWAction->setShortcut(tr("Delete"));
    suggestDTWAction->setStatusTip(tr("suggest matching by DTW"));
    connect(suggestDTWAction, SIGNAL(triggered()), this, SLOT(suggestDTW()));
}

void MatchingFragmentWindow::createHelpMenuActions()
{
    aboutAction = new QAction(tr("&About MAELab"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
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

void MatchingFragmentWindow::rotateleft()
{
    FragmentItem *selected = selectedFragmentItem();
    if(selected){
        rotateCenter(selected,-1);
    }
}

void MatchingFragmentWindow::rotateright()
{
    FragmentItem *selected = selectedFragmentItem();
    if(selected){
        rotateCenter(selected,+1);
    }
}

void MatchingFragmentWindow::setLeftFragment()
{
    leftFragment = selectedFragmentItem();
    if(leftFragment){
        cout << "set left" << endl;
    }
}

void MatchingFragmentWindow::setRightFragment()
{
    rightFragment = selectedFragmentItem();
    if(rightFragment){
        cout << "set right" << endl;
    }
}

void MatchingFragmentWindow::suggestDTW()
{
    if(leftFragment && rightFragment){
        cout << "DTW" << endl;

        // get list points from Left
        cout << leftFragment->getObjectRGBAMatrix()->getRows() << endl;
        cout << rightFragment->getObjectRGBAMatrix()->getRows() << endl;
        vector<Point> listPointsOfLeft = findMinXmappingY(leftFragment->getBorder());
        vector<Point> listPointsOfRight = findMaxXmappingY(rightFragment->getBorder());

        // get list points from Rght

        // run the algorithm
        vector<int> vectorX;
        vector<int> vectorY;
        vector< vector<int> > myDTW = cumulativeDistanceMatrix(vectorX,vectorY);
        vector< pair<int,int> > myPath = optimalWarpingPath(myDTW);
        int n = myPath.size();
        for(int i=0; i<n; i++)
        {
            cout << myPath[i].first << "\t" << myPath[i].second << endl;
        }
    }
}

void MatchingFragmentWindow::loadObject(ptrRGBAMatrix objectRGBAMatrix, Edge border)
{
    qImage = ptrRGBAToQImage(objectRGBAMatrix);

    FragmentItem *pixmapItem = new FragmentItem(border, objectRGBAMatrix, QPixmap::fromImage(qImage), fragmentMenu);

    scene->addItem(pixmapItem);
}

void MatchingFragmentWindow::createActions()
{
    createFileMenuActions();
    createFragmentMenuActions();
    createMatchingMenuActions();
    createHelpMenuActions();
}

void MatchingFragmentWindow::createFileMenuActions()
{
    openAction = new QAction(QIcon(":/Icons/resources/ico/open.png"), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void MatchingFragmentWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    fragmentMenu = menuBar()->addMenu(tr("&Fragment"));
    fragmentMenu->addAction(deleteAction);
    fragmentMenu->addSeparator();
    fragmentMenu->addAction(toFrontAction);
    fragmentMenu->addAction(sendBackAction);
    fragmentMenu->addSeparator();
    fragmentMenu->addAction(rotateLeftAction);
    fragmentMenu->addAction(rotateRightAction);
    fragmentMenu->addSeparator();
    fragmentMenu->addAction(setLeftFragmentAction);
    fragmentMenu->addAction(setRightFragmentAction);

    matchingMenu = menuBar()->addMenu(tr("&Matching"));
    matchingMenu->addAction(suggestDTWAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);

    // add menus to GUI
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(fragmentMenu);
    menuBar()->addMenu(matchingMenu);
    menuBar()->addMenu(helpMenu);
}

void MatchingFragmentWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAction);

    fragmentToolBar = addToolBar(tr("Fragment"));
    fragmentToolBar->addAction(deleteAction);
    fragmentToolBar->addAction(toFrontAction);
    fragmentToolBar->addAction(sendBackAction);
    fragmentToolBar->addAction(rotateLeftAction);
    fragmentToolBar->addAction(rotateRightAction);
    fragmentToolBar->addAction(setLeftFragmentAction);
    fragmentToolBar->addAction(setRightFragmentAction);

    matchingToolBar = addToolBar(tr("Matching"));
    matchingToolBar->addAction(suggestDTWAction);

}

vector<Point> MatchingFragmentWindow::findMinXmappingY(Edge edge)
{
    vector<Point> result;
    edge.sortByY();
    vector<Point> theListPoints = edge.getPoints();
    int size = theListPoints.size();
    Point minPoint = theListPoints.at(0);
    for(int i = 1;i<size;i++)
    {
        Point currentPoint = theListPoints.at(i);
        // compare the same y
        if(minPoint.getY() == currentPoint.getY())
        {
            // find the min X
            if(minPoint.getX()>currentPoint.getX())
            {
                minPoint = currentPoint;
            }
        }
        else
        {
            result.push_back(minPoint);
            minPoint = currentPoint;
        }
    }
    // add point of final range
    result.push_back(minPoint);
    return result;
}

vector<Point> MatchingFragmentWindow::findMaxXmappingY(Edge edge)
{
    vector<Point> result;
    edge.sortByY();
    vector<Point> theListPoints = edge.getPoints();
    int size = theListPoints.size();
    Point maxPoint = theListPoints.at(0);
    for(int i = 1;i<size;i++)
    {
        Point currentPoint = theListPoints.at(i);
        // compare the same y
        if(maxPoint.getY() == currentPoint.getY())
        {
            // find the max X
            if(maxPoint.getX()<currentPoint.getX())
            {
                maxPoint = currentPoint;
            }
        }
        else
        {
            result.push_back(maxPoint);
            maxPoint = currentPoint;
        }
    }
    // add point of final range
    result.push_back(maxPoint);
    return result;
}
