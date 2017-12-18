/*
 * ImageViewer.h
 *
 *  Created on: Dec 15, 2016
 *      Author: linh
 */

#ifndef IMAGEVIEWER_H_
#define IMAGEVIEWER_H_

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
#include "segmentation/RegionGrowing.h"
#include "ImageLabel.h"

#include <QtGui/qlabel.h>
#include <QtGui/qscrollarea.h>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QApplication>
#include <QtGui/QScrollBar>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPrintDialog>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QPainter>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/qwidget.h>

class Image;

class ImageViewer: public QMainWindow
{
Q_OBJECT
public:
	ImageViewer();
	~ImageViewer();

	QString fileName;
	QImage qImage;
        Image *matImage;

	void loadImage(QString fn);
	void loadImage(Image *_matImage, QImage _qImage, QString tt);

signals:
    void sendObjectRGBA(string imageName, int locationX, int locationY, ptrRGBAMatrix objectRGBAMatrix, Edge border);

private slots:
    void open();
    void extractObject(int x, int y);

private:

    ImageLabel *imageLabel;
    QScrollArea *scrollArea;

	// menu
        QMenu *fileMenu;

	// toolbar
        QToolBar *fileToolBar;

	//menu action
	QAction *openAct;

	// private methods

	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createFileMenu();


};

#endif /* IMAGEVIEWER_H_ */
