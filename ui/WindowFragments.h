#ifndef WINDOWFRAGMENTS_H
#define WINDOWFRAGMENTS_H

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
#include <string>
using namespace std;

#include "../MAELab/imageModel/Point.h"
#include "../MAELab/imageModel/Line.h"
#include "../MAELab/imageModel/Edge.h"
#include "../MAELab/imageModel/Matrix.h"
#include "../MAELab/imageModel/Image.h"
#include "../MAELab/io/Reader.h"
#include "../MAELab/segmentation/Thresholds.h"
#include "../MAELab/segmentation/Canny.h"
#include "../MAELab/segmentation/Suzuki.h"
#include "../MAELab/segmentation/Filters.h"
#include "../MAELab/utils/ImageConvert.h"
#include "algorithm/MatchingAlgorithm.h"
#include "ImageViewer.h"
#include "FragmentItem.h"
#include "SceneFragments.h"

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

class Image;

class WindowFragments : public QMainWindow
{
    Q_OBJECT

public:
    WindowFragments();
    ~WindowFragments();

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
    void propertyItem();
    void suggestDTW();

public slots:
    void loadObject(string imageName, int locationX, int locationY, ptrRGBAMatrix objectRGBAMatrix, Edge border);

private:
    // rotate around center point with angle
    void rotateCenter(QGraphicsItem *selectedItem, int angle);
    void createActions();
    void createFileMenuActions();
    void createFragmentMenuActions();
    void createMatchingMenuActions();
    void createHelpMenuActions();

    void createMenus();
    void createToolBars();

    SceneFragments *scene;
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
    QAction *propertyAction;

    // actions
    QAction *openAction;
    QAction *exitAction;
    QAction *aboutAction;

    // fragment items left and right
    FragmentItem *leftFragment;
    FragmentItem *rightFragment;

    // image viewer
    ImageViewer *imageViewer;

    QGraphicsItem *selectedItem() const;
};

#endif // WINDOWFRAGMENTS_H
