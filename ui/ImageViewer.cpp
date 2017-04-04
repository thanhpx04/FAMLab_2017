/*
 * ImageViewer.cpp
 *
 *  Created on: Dec 15, 2016
 *      Author: linh
 */
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

#include "../pht/GHTInPoint.h"
#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../pht/PCA.h"
#include "../pht/SVD.h"

#include "../histograms/ShapeHistogram.h"
#include "../pointInterest/Treatments.h"
#include "../pointInterest/Segmentation.h"
#include "../pointInterest/ProHoughTransform.h"
#include "../pointInterest/LandmarkDetection.h"
#include "../utils/ImageConvert.h"
#include "../utils/Drawing.h"
#include "../utils/Converter.h"
#include "../correlation/CrossCorrelation.h"
#include "../correlation/DescriptorDistance.h"

#include "../MAELab.h"

#include "ImageViewer.h"

void ImageViewer::createFileMenu()
{
	openAct = new QAction(QIcon("./resources/ico/open.png"), tr("&Open..."),
		this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon("./resources/ico/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	saveAct->setEnabled(false);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	saveAsAct->setEnabled(false);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	closeAct = new QAction(tr("&Close"), this);
	closeAct->setShortcut(tr("Ctrl+W"));
	closeAct->setStatusTip(tr("Close this window"));
	connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

}
void ImageViewer::createViewMenu()
{
	zoomInAct = new QAction(QIcon("./resources/ico/1uparrow.png"),
		tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(QIcon("./resources/ico/1downarrow.png"),
		tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+N"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+J"));
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

	gHistogramAct = new QAction(tr("&Histogram"), this);
	gHistogramAct->setEnabled(false);
	connect(gHistogramAct, SIGNAL(triggered()), this, SLOT(gScaleHistogram()));

	displayMLandmarksAct = new QAction(tr("&Display manual landmarks"), this);
	displayMLandmarksAct->setEnabled(false);
	displayMLandmarksAct->setCheckable(true);
	displayMLandmarksAct->setChecked(false);
	connect(displayMLandmarksAct, SIGNAL(triggered()), this,
		SLOT(displayManualLandmarks()));

	displayALandmarksAct = new QAction(tr("&Display estimated landmarks"), this);
	displayALandmarksAct->setEnabled(false);
	displayALandmarksAct->setCheckable(true);
	connect(displayALandmarksAct, SIGNAL(triggered()), this,
		SLOT(displayAutoLandmarks()));

}
void ImageViewer::viewMenuUpdateActions()
{
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
	Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(
		int(
			factor * scrollBar->value()
				+ ((factor - 1) * scrollBar->pageStep() / 2)));
}

void ImageViewer::createHelpMenu()
{
	aboutAct = new QAction(tr("&About MAELab"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	testAct = new QAction(tr("&Test a function"), this);
	testAct->setEnabled(false);
	connect(testAct, SIGNAL(triggered()), this, SLOT(testMethod()));
}
void ImageViewer::createSegmentationMenu()
{
	binaryThresholdAct = new QAction(tr("&Binary threshold"), this);
	binaryThresholdAct->setEnabled(false);
	connect(binaryThresholdAct, SIGNAL(triggered()), this, SLOT(binThreshold()));

	cannyAct = new QAction(tr("&Canny result"), this);
	cannyAct->setEnabled(false);
	connect(cannyAct, SIGNAL(triggered()), this, SLOT(cannyAlgorithm()));

	suzukiAct = new QAction(tr("&Suzuki result"), this);
	suzukiAct->setEnabled(false);
	connect(suzukiAct, SIGNAL(triggered()), this, SLOT(suzukiAlgorithm()));

	lineSegmentationAct = new QAction(tr("&View segmentation result"), this);
	lineSegmentationAct->setEnabled(false);
	connect(lineSegmentationAct, SIGNAL(triggered()), this,
		SLOT(lineSegmentation()));
}
void ImageViewer::createLandmarksMenu()
{
	/*phtAct = new QAction(tr("&Probabilistic Hough Transform"), this);
	 phtAct->setEnabled(false);
	 phtAct->setShortcut(tr("Ctrl+P"));
	 connect(phtAct, SIGNAL(triggered()), this, SLOT(pHoughTransform()));*/

	phtPointsAct = new QAction(tr("&Generalizing Hough Transform"), this);
	phtPointsAct->setEnabled(false);
	phtPointsAct->setShortcut(tr("Ctrl+G"));
	connect(phtPointsAct, SIGNAL(triggered()), this, SLOT(gHoughTransform()));

	autoLandmarksAct = new QAction(tr("&Probabilistic Hough Transform"), this);
	autoLandmarksAct->setEnabled(false);
	autoLandmarksAct->setShortcut(tr("Ctrl+L"));
	connect(autoLandmarksAct, SIGNAL(triggered()), this,
		SLOT(extractLandmarks()));

	pcaiAct = new QAction(tr("PCAI method"), this);
	pcaiAct->setEnabled(false);
	pcaiAct->setShortcut(tr("Ctrl+I"));
	connect(pcaiAct, SIGNAL(triggered()), this, SLOT(pcaiMethodViewer()));

	measureMBaryAct = new QAction(tr("&Measure manual centroid"), this);
	measureMBaryAct->setEnabled(false);
	connect(measureMBaryAct, SIGNAL(triggered()), this, SLOT(measureMBary()));

	measureEBaryAct = new QAction(tr("&Measure estimated centroid"), this);
	measureEBaryAct->setEnabled(false);
	connect(measureEBaryAct, SIGNAL(triggered()), this, SLOT(measureEBary()));

	dirAutoLandmarksAct = new QAction(tr("Compute automatic landmarks on folder"),
		this);
	dirAutoLandmarksAct->setEnabled(false);
	connect(dirAutoLandmarksAct, SIGNAL(triggered()), this,
		SLOT(dirAutoLandmarks()));

	dirCentroidMeasureAct = new QAction(tr("Measure centroid on folder"), this);
	dirCentroidMeasureAct->setEnabled(false);
	connect(dirCentroidMeasureAct, SIGNAL(triggered()), this,
		SLOT(dirCentroidMeasure()));

	dirGenerateDataAct = new QAction(tr("Random generated the data"), this);
	dirGenerateDataAct->setEnabled(false);
	dirGenerateDataAct->setShortcut(tr("Ctrl+D"));
	connect(dirGenerateDataAct, SIGNAL(triggered()), this,
		SLOT(dirGenerateData()));
}
/*void ImageViewer::createRegistrationMenu()
 {
 icpAct = new QAction(tr("PCAI method"), this);
 icpAct->setEnabled(false);
 icpAct->setShortcut(tr("Ctrl+I"));
 connect(icpAct, SIGNAL(triggered()), this, SLOT(icpMethodViewer()));
 }*/

void ImageViewer::createActions()
{
	createFileMenu();
	createViewMenu();
	createHelpMenu();
	createSegmentationMenu();
	createLandmarksMenu();
	//createRegistrationMenu();
}
void ImageViewer::createMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAct);
	fileMenu->addAction(exitAct);

	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addSeparator();
	viewMenu->addAction(normalSizeAct);
	viewMenu->addAction(fitToWindowAct);
	viewMenu->addSeparator();
	viewMenu->addAction(gHistogramAct);
	viewMenu->addAction(displayMLandmarksAct);
	viewMenu->addAction(displayALandmarksAct);

	helpMenu = new QMenu(tr("&Helps"), this);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(testAct);

	segmentationMenu = new QMenu(tr("&Segmentation"), this);
	segmentationMenu->addAction(binaryThresholdAct);
	segmentationMenu->addAction(cannyAct);
	segmentationMenu->addAction(suzukiAct);
	segmentationMenu->addAction(lineSegmentationAct);

	dominantPointMenu = new QMenu(tr("&Landmarks"), this);
	//dominantPointMenu->addAction(phtAct);
	dominantPointMenu->addAction(phtPointsAct);
	dominantPointMenu->addAction(autoLandmarksAct);
	dominantPointMenu->addAction(pcaiAct);
	dominantPointMenu->addSeparator();
	dominantPointMenu->addAction(measureMBaryAct);
	dominantPointMenu->addAction(measureEBaryAct);
	dominantPointMenu->addSeparator();
	QMenu* menuDirectory = dominantPointMenu->addMenu(tr("Working on directory"));
	menuDirectory->addAction(dirAutoLandmarksAct);
	menuDirectory->addAction(dirCentroidMeasureAct);
	menuDirectory->addAction(dirGenerateDataAct);

	//registrationMenu = new QMenu(tr("&Registration"), this);
	//registrationMenu->addAction(icpAct);

	// add menus to GUI
	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(segmentationMenu);
	menuBar()->addMenu(dominantPointMenu);
	//menuBar()->addMenu(registrationMenu);
	menuBar()->addMenu(helpMenu);
}
void ImageViewer::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	viewToolBar = addToolBar(tr("View"));
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
}
void ImageViewer::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}
void ImageViewer::activeFunction()
{
	openAct->setEnabled(true);
	saveAct->setEnabled(true);
	saveAsAct->setEnabled(true);

	zoomInAct->setEnabled(true);
	zoomOutAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	normalSizeAct->setEnabled(true);
	gHistogramAct->setEnabled(true);
	displayMLandmarksAct->setEnabled(true);
	if (matImage->getListOfAutoLandmarks().size() > 0)
		displayALandmarksAct->setEnabled(true);

	binaryThresholdAct->setEnabled(true);
	cannyAct->setEnabled(true);
	suzukiAct->setEnabled(true);
	lineSegmentationAct->setEnabled(true);

	//phtAct->setEnabled(true);
	phtPointsAct->setEnabled(true);
	autoLandmarksAct->setEnabled(true);
	if (matImage->getListOfManualLandmarks().size() > 0)
		measureMBaryAct->setEnabled(true);
	if (matImage->getListOfAutoLandmarks().size() > 0)
		measureEBaryAct->setEnabled(true);
	dirAutoLandmarksAct->setEnabled(true);
	dirCentroidMeasureAct->setEnabled(true);
	dirGenerateDataAct->setEnabled(true);

	pcaiAct->setEnabled(true);

	testAct->setEnabled(true);

	viewMenuUpdateActions();
	if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();
}

ImageViewer::ImageViewer()
{
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	setCentralWidget(scrollArea);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	setWindowTitle(tr(".: MAELab :."));
	resize(900, 700);

	setWindowIcon(QIcon("./resources/ico/ip.ico"));
	//parameterPanel = NULL;

}

ImageViewer::~ImageViewer()
{
	delete matImage;
	delete parameterAction;
	delete parameterDialog;

	delete imageLabel;
	delete scrollArea;

	// menu
	delete fileMenu;
	delete viewMenu;
	delete segmentationMenu;
	delete dominantPointMenu;
	delete helpMenu;

	// toolbar
	delete fileToolBar;
	delete viewToolBar;

	//menu action
	delete openAct;
	delete printAct;
	delete saveAct;
	delete saveAsAct;
	delete closeAct;
	delete exitAct;

	delete zoomInAct;
	delete zoomOutAct;
	delete normalSizeAct;
	delete fitToWindowAct;
	delete displayMLandmarksAct;
	delete displayALandmarksAct;

	delete aboutAct;

	delete binaryThresholdAct;
	delete cannyAct;
	delete suzukiAct;
	delete lineSegmentationAct;

	//delete phtAct;
	delete phtPointsAct;
	delete autoLandmarksAct;
	delete measureMBaryAct;
	delete measureEBaryAct;
	delete dirAutoLandmarksAct;
	delete dirCentroidMeasureAct;
	delete dirGenerateDataAct;

	//delete icpAct;
}

void ImageViewer::loadImage(QString fn)
{

	matImage = new Image(fn.toStdString());
	qImage.load(fn);

	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	scaleFactor = 1.0;

	saveAsAct->setEnabled(true);
	activeFunction();

	this->fileName = fn;
	setWindowTitle(tr("Image Viewer - ") + fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}
void ImageViewer::loadImage(Image *_matImage, QImage _qImage, QString tt)
{
	matImage = _matImage;
	qImage = _qImage;
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	scaleFactor = 1.0;

	saveAct->setEnabled(true);
	activeFunction();

	setWindowTitle(tr("Image Viewer - ") + tt);
	statusBar()->showMessage(tr("Finished"), 2000);
}

void ImageViewer::displayLandmarks(Image *image, vector<Point> lms, RGB color)
{
	Point lm;
	for (size_t i = 0; i < lms.size(); i++)
	{
		lm = lms.at(i);
		cout << "\nManual landmark: " << lm.getX() << "\t" << lm.getY();
		fillCircle(*(image->getRGBMatrix()), lm, 3, color);

	}

}
// =========================== Slots =======================================
void ImageViewer::about()
{
	QMessageBox::about(this, tr("About MAELab"),
		tr(
			"<p><b>MAELab</b> is a software in Computer Vision. It provides the function to "
				"segmentation and detection dominant points.</p>"));
}
void ImageViewer::testMethod()
{
	cout << "\nTest a method ..." << endl;
	//matImage->readManualLandmarks("data/landmarks/Md 054.TPS");
	ptr_IntMatrix rchannel = new Matrix<int>(matImage->getGrayMatrix()->getRows(),
		matImage->getGrayMatrix()->getCols(), 0);
	ptr_IntMatrix gchannel = new Matrix<int>(matImage->getGrayMatrix()->getRows(),
		matImage->getGrayMatrix()->getCols(), 0);
	for (int r = 0; r < matImage->getGrayMatrix()->getRows(); r++)
	{
		for (int c = 0; c < matImage->getGrayMatrix()->getCols(); c++)
		{
			RGB color = matImage->getRGBMatrix()->getAtPosition(r, c);
			rchannel->setAtPosition(r, c, color.R);
			gchannel->setAtPosition(r, c, color.G);
		}
	}
	ptr_IntMatrix rcontours = getContour(rchannel);
	ptr_IntMatrix gcontours = getContour(gchannel);

	ptr_IntMatrix hMatrix = new Matrix<int>(matImage->getGrayMatrix()->getRows(),
	 matImage->getGrayMatrix()->getCols(), 0);
	int minValue = DBL_MAX, maxValue = DBL_MIN;
	for (int r = 0; r < gcontours->getRows(); r++)
	{
		for (int c = 0; c < gcontours->getCols(); c++)
		{
			if (gcontours->getAtPosition(r, c) < minValue)
				minValue = gcontours->getAtPosition(r, c);
			if (gcontours->getAtPosition(r, c) > maxValue)
				maxValue = gcontours->getAtPosition(r, c);
		}
	}
	int mid = (minValue + maxValue) / 2;
	cout << endl << "Min max in contours: " << minValue << "\t" << maxValue << "\t"<<mid
		<< endl;


	//vector<Edge> edges = test2(*matImage);
	RGB color;
	color.R = 255;
	color.G = color.B = 0;
	Edge edgei;
	Point pi;
	int rows = matImage->getGrayMatrix()->getRows();
	int cols = matImage->getGrayMatrix()->getCols();
	int count = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (gcontours->getAtPosition(r, c) > 3)
			{
				//matImage->getRGBMatrix()->setAtPosition(r, c, color);
				hMatrix->setAtPosition(r,c,255);
				count++;
			}
		}
	}
	cout<<endl<<count<<endl;
	saveGrayScale("hbcMatrix.jpg", hMatrix);
	vector<Point> corner = boundingBoxDetection(gchannel);
	fillCircle(*matImage->getRGBMatrix(),corner.at(0),7,color);
	fillCircle(*matImage->getRGBMatrix(),corner.at(1),7,color);
	fillCircle(*matImage->getRGBMatrix(),corner.at(2),7,color);
	fillCircle(*matImage->getRGBMatrix(),corner.at(3),7,color);
	/*ptr_IntMatrix quant = quantization(matImage->getGrayMatrix(),3);

	 vector<Point> corners = boundingBoxDetection(matImage->getGrayMatrix());
	 Point p1 = corners.at(0), p2 = corners.at(1), p3 = corners.at(2), p4 =
	 corners.at(3);
	 RGB color;
	 color.R = color.G = 0;
	 color.B = 255;
	 drawingLine(*matImage->getRGBMatrix(), Line(p1, p2), color);
	 drawingLine(*matImage->getRGBMatrix(), Line(p1, p3), color);
	 drawingLine(*matImage->getRGBMatrix(), Line(p2, p4), color);
	 drawingLine(*matImage->getRGBMatrix(), Line(p3, p4), color);*/

	/*Image scene("data/md028.jpg");
	 scene.readManualLandmarks("data/landmarks/Md 028.TPS");
	 matImage->readManualLandmarks("data/landmarks/Md 028.TPS");
	 vector<Point> rslm = test(scene, *matImage,
	 matImage->getListOfManualLandmarks());
	 RGB color;
	 color.R = color.G = 0;
	 color.B = 255;
	 for (int i = 0; i < rslm.size(); ++i)
	 {
	 rslm.at(i).toString();
	 fillCircle(*matImage->getRGBMatrix(), rslm.at(i), 5, color);
	 }*/
	//this->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()), "Test");
	//this->show();
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
		"Quantization");
	other->move(x() - 40, y() - 40);
	other->show();

	cout << "\nFinish.\n";
	cout << "\n End test method!" << endl;
}
void ImageViewer::open()
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
		if (!this->fileName.isEmpty())
		{
			ImageViewer* other = new ImageViewer;
			other->loadImage(fileName);
			other->move(x() + 40, y() + 40);
			other->show();
		}
		else
		{
			this->loadImage(fileName);
		}
	}
}
void ImageViewer::save()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), ".",
		tr("Image Files (*.jpg)"));
	if (fileName.isEmpty())
	{
		cout << "\nCan not save the image !!";
		return;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	qImage.save(fileName, "JPEG");

	QApplication::restoreOverrideCursor();

	this->fileName = fileName;
	setWindowTitle(tr("MAELab - ") + fileName);

	saveAct->setEnabled(false);
	saveAsAct->setEnabled(true);

	statusBar()->showMessage(tr("File saved"), 2000);
	return;
}
void ImageViewer::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), ".",
		tr("Image Files (*.jpg)"));
	if (fileName.isEmpty())
	{
		cout << "\nCan not save the image !!";
		return;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	qImage.save(fileName, "JPEG");
	QApplication::restoreOverrideCursor();

	saveAct->setEnabled(true);

	statusBar()->showMessage(tr("File saved"), 2000);
	return;
}
void ImageViewer::zoomIn()
{
	scaleImage(1.25);
}
void ImageViewer::zoomOut()
{
	scaleImage(0.8);
}
void ImageViewer::normalSize()
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}
void ImageViewer::fitToWindow()
{

	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow)
	{
		normalSize();
	}
	viewMenuUpdateActions();
}
void ImageViewer::gScaleHistogram()
{
	cout << "\n Gray scale histogram." << endl;
	ptr_IntMatrix histogram = matImage->getHistogram();
	int max = -1;
	for (int c = 0; c < histogram->getCols(); c++)
	{
		if (histogram->getAtPosition(0, c) > max)
			max = histogram->getAtPosition(0, c);
	}
	int cols = histogram->getCols();
	RGB color;
	color.R = color.G = color.B = 0;
	ptr_RGBMatrix hDisplay = new Matrix<RGB>(240, 300, color);
	Point pbegin, pend;
	color.R = color.G = color.B = 255;
	for (int c = 0; c < histogram->getCols(); c++)
	{
		pbegin.setX(c);
		pbegin.setY(239);
		pend.setX(c);
		pend.setY(239 - (histogram->getAtPosition(0, c) * 230 / max));
		drawingLine(*hDisplay, Line(pbegin, pend), color);
	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptrRGBToQImage(hDisplay), "Histogram result");
	other->move(x() - 40, y() - 40);
	other->show();
	cout << "\nFinished." << endl;
}
void ImageViewer::displayManualLandmarks()
{
	cout << "\n Display the manual landmarks.\n";

	bool currentState = displayMLandmarksAct->isChecked();
	if (currentState)
	{
		if (matImage->getListOfManualLandmarks().size() <= 0)
		{
			QMessageBox msgbox;
			msgbox.setText("Select the manual landmarks file.");
			msgbox.exec();

			QString reflmPath = QFileDialog::getOpenFileName(this);
			matImage->readManualLandmarks(reflmPath.toStdString());

		}
		vector<Point> mLandmarks = matImage->getListOfManualLandmarks();
		RGB color;
		color.R = 255;
		color.G = 0;
		color.B = 0;
		displayLandmarks(matImage, mLandmarks, color);
		displayMLandmarksAct->setChecked(true);
		measureMBaryAct->setEnabled(true);
	}
	else
	{
		Image *img = new Image(fileName.toStdString());
		matImage->setRGBMatrix(img->getRGBMatrix());
		displayMLandmarksAct->setChecked(false);
	}

	if (displayALandmarksAct->isChecked())
	{
		vector<Point> aLM = matImage->getListOfAutoLandmarks();
		if (aLM.size() > 0)
		{
			RGB color;
			color.R = 255;
			color.G = 255;
			color.B = 0;
			displayLandmarks(matImage, aLM, color);
			displayALandmarksAct->setChecked(true);
		}
	}

	this->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
		"Display manual landmarks.");
	this->show();
	cout << "\nFinish.\n";
}
void ImageViewer::displayAutoLandmarks()
{
	vector<Point> autoLM = matImage->getListOfAutoLandmarks();
	QMessageBox message;
	if (autoLM.size() <= 0)
	{
		message.setText(
			"Automatic landmarks do not exists. You need to compute them.");
		message.exec();
	}
	else
	{
		bool currentState = displayALandmarksAct->isChecked();
		if (currentState)
		{
			RGB color;
			color.R = 255;
			color.G = 255;
			color.B = 0;
			displayLandmarks(matImage, autoLM, color);
			displayALandmarksAct->setChecked(true);
		}
		else
		{
			Image *img = new Image(fileName.toStdString());
			matImage->setRGBMatrix(img->getRGBMatrix());
			displayALandmarksAct->setChecked(false);
		}

		if (displayMLandmarksAct->isChecked())
		{
			vector<Point> mLM = matImage->getListOfManualLandmarks();
			if (mLM.size() > 0)
			{
				RGB color;
				color.R = 255;
				color.G = 0;
				color.B = 0;
				displayLandmarks(matImage, mLM, color);
				displayMLandmarksAct->setChecked(true);
			}
		}
		this->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
			"Display estimated landmarks.");
	}

}
void ImageViewer::binThreshold()
{
	cout << "\nBinary thresholding...\n";
	float tValue = matImage->getThresholdValue();

	Segmentation tr; // = new Segmentation();
	tr.setRefImage(*matImage);
	cout << "\ntValue: " << tValue << endl;
	ptr_IntMatrix rsMatrix = tr.threshold(tValue, 255);
	rsMatrix = postProcess(rsMatrix,255);

	//ptr_IntMatrix hProjection = new Matrix<int>(rsMatrix->getRows(),rsMatrix->getCols(),255);
	//ptr_IntMatrix vProjection(hProjection);
	//binProjection(rsMatrix,hProjection,vProjection);
	//saveGrayScale("hProjection.jpg",hProjection);
	//saveGrayScale("vProjection.jpg",vProjection);



	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptrIntToQImage(rsMatrix), "Thresholding result");
	other->show();

}
void ImageViewer::cannyAlgorithm()
{
	cout << "\nCanny Algorithm...\n";
	Segmentation tr;
	tr.setRefImage(*matImage);
	vector<Edge> edges = tr.canny();

	RGB color;
	color.R = 255;
	color.G = color.B = 0;
	Edge edgei;
	Point pi;
	int rows = matImage->getGrayMatrix()->getRows();
	int cols = matImage->getGrayMatrix()->getCols();

	for (size_t i = 0; i < edges.size(); i++)
	{
		edgei = edges.at(i);
		for (size_t k = 0; k < edgei.getPoints().size(); k++)
		{
			pi = edgei.getPoints().at(k);
			if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
				&& pi.getY() < rows)
			{
				matImage->getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(), color);
			}
		}
	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
		"Canny result");
	other->move(x() - 40, y() - 40);
	other->show();
}
void ImageViewer::suzukiAlgorithm()
{
	cout << "\nSuzuki Algorithm...\n";
	Segmentation tr;
	tr.setRefImage(*matImage);
	vector<Edge> edges = tr.canny();

	RGB color;
	color.R = 255;
	color.G = color.B = 0;
	Edge edgei;
	Point pi;
	int rows = matImage->getGrayMatrix()->getRows();
	int cols = matImage->getGrayMatrix()->getCols();

	for (size_t i = 0; i < edges.size(); i++)
	{
		edgei = edges.at(i);
		for (size_t k = 0; k < edgei.getPoints().size(); k++)
		{
			pi = edgei.getPoints().at(k);
			if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
				&& pi.getY() < rows)
			{
				matImage->getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(), color);
			}
		}
	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
		"Canny result");
	other->move(x() - 40, y() - 40);
	other->show();
}
void ImageViewer::lineSegmentation()
{
	cout << "\nDisplay the list of lines." << endl;
	QMessageBox msgbox;
	Segmentation tr;
	tr.setRefImage(*matImage);
	vector<Line> listOfLines = tr.segment();
	RGB color;
	color.R = 255;
	color.G = color.B = 0;
	Line linei;
	for (size_t i = 0; i < listOfLines.size(); i++)
	{
		linei = listOfLines.at(i);
		drawingLine(*(matImage->getRGBMatrix()), linei, color);
	}

	this->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
		"Landmarks result");
	this->show();

	msgbox.setText("Finish");
	msgbox.exec();
}
// ======================================================= Landmarks points ===============================
void ImageViewer::gHoughTransform()
{
	cout << "\n Generalizing hough transform." << endl;
	QMessageBox msgbox;

	// extract list of points of model image by using canny
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	modelImage->readManualLandmarks(reflmPath.toStdString());
	//int rows = matImage->getGrayMatrix()->getRows();
	//int cols = matImage->getGrayMatrix()->getCols();
	ProHoughTransform tr;
	tr.setRefImage(*modelImage);

	Point ePoint, mPoint;
	//double angleDiff;
	//==================================================================
	LandmarkDetection lmDetect;
	lmDetect.setRefImage(*modelImage);
	//vector<Point> estLandmarks = lmDetect.landmarksAutoDectect2(*matImage, 9, 36);
	vector<Point> estLandmarks = lmDetect.landmarksWithSIFT(*matImage, 9, 36);
	cout << "\nNumber of the landmarks: " << estLandmarks.size() << endl;
	// drawing...
	Point lm;
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::yellow);
	qpainter.setBrush(Qt::yellow);
	qpainter.setFont(QFont("Arial", 15));
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.getX() << "\t" << lm.getY()
			<< endl;
		//fillCircle(*(matImage->getRGBMatrix()), lm, 5, color);
		if (lm.getX() >= 0 && lm.getX() < matImage->getRGBMatrix()->getCols()
			&& lm.getY() >= 0 && lm.getY() < matImage->getRGBMatrix()->getRows())
		{
			qpainter.drawEllipse(lm.getX(), lm.getY(), 4, 4);
			qpainter.drawText(lm.getX() + 6, lm.getY(), QString::number((int) i));
		}
	}
	qpainter.end();
	Point ebary;
	double mCentroid = measureCentroidPoint(estLandmarks, ebary);
	msgbox.setText(
		"<p>Coordinate of bary point: (" + QString::number(ebary.getX()) + ", "
			+ QString::number(ebary.getY()) + ")</p>"
				"<p>Centroid value: " + QString::number(mCentroid) + "</p");
	msgbox.exec();

	this->loadImage(matImage, qImage, "Landmarks result");
	this->show();
	//delete newScene;
	msgbox.setText("Finish.");
	msgbox.exec();

}

void ImageViewer::extractLandmarks()
{
	cout << "\n Automatic extraction the landmarks.\n";
	QMessageBox msgbox;
	msgbox.setText("Select the model image.");
	msgbox.exec();

	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	modelImage->readManualLandmarks(reflmPath.toStdString());

	LandmarkDetection tr;
	tr.setRefImage(*modelImage);

	Point ePoint;
	double angleDiff;
	vector<Point> lms = tr.landmarksAutoDectect(*matImage, Degree, 500, 400, 500,
		ePoint, angleDiff);
	cout << "\nNumber of the landmarks: " << lms.size() << endl;
	RGB color;
	color.R = 255;
	color.G = 255;
	color.B = 0;
	Point lm;
	matImage->rotate(ePoint, angleDiff, 1);
	for (size_t i = 0; i < lms.size(); i++)
	{
		lm = lms.at(i);
		drawingCircle(*(matImage->getRGBMatrix()), lm, 5, color);
	}
	matImage->setAutoLandmarks(lms);
	displayALandmarksAct->setEnabled(true);
	displayALandmarksAct->setChecked(true);
	measureEBaryAct->setEnabled(true);
	this->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
		"Landmarks result");
	this->show();

	msgbox.setText("Finish");
	msgbox.exec();
}
void ImageViewer::measureMBary()
{
	QMessageBox qmessage;
	vector<Point> mLandmarks = matImage->getListOfManualLandmarks();
	if (mLandmarks.size() > 0)
	{
		Point ebary(0, 0);
		double mCentroid = measureCentroidPoint(mLandmarks, ebary);

		qmessage.setText(
			"<p>Coordinate of bary point: (" + QString::number(ebary.getX()) + ", "
				+ QString::number(ebary.getY()) + ")</p>"
					"<p>Centroid value: " + QString::number(mCentroid) + "</p");
	}
	else
	{
		qmessage.setText("The image has not the manual landmarks.");
	}
	qmessage.exec();
}
void ImageViewer::measureEBary()
{
	QMessageBox qmessage;
	vector<Point> mLandmarks = matImage->getListOfAutoLandmarks();
	if (mLandmarks.size() > 0)
	{
		Point ebary(0, 0);
		double mCentroid = measureCentroidPoint(mLandmarks, ebary);

		qmessage.setText(
			"<p>Coordinate of bary point: (" + QString::number(ebary.getX()) + ", "
				+ QString::number(ebary.getY()) + ")</p>"
					"<p>Centroid value: " + QString::number(mCentroid) + "</p");
	}
	else
	{
		qmessage.setText("The image has not the automatic landmarks.");
	}
	qmessage.exec();
}
void ImageViewer::dirAutoLandmarks()
{
	cout << "\n Automatic estimated landmarks on directory." << endl;
	QMessageBox msgbox;

	msgbox.setText("Select the model's landmarks file");
	msgbox.exec();
	QString lpath = QFileDialog::getOpenFileName(this);
	matImage->readManualLandmarks(lpath.toStdString());

	msgbox.setText("Selecte the scene images folder.");
	msgbox.exec();
	QString folder = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Selecte the saving folder.");
	msgbox.exec();
	QString savefolder = QFileDialog::getExistingDirectory(this);

	vector<string> fileNames = readDirectory(folder.toStdString().c_str());
	Point pk;
	vector<Point> esLandmarks;
	Point ePoint(0, 0);
	double angleDiff = 0;
	LandmarkDetection tr;
	tr.setRefImage(*matImage);

	string fileName;
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		fileName = folder.toStdString() + "/" + fileNames.at(i);
		cout << "\n" << fileName << endl;
		Image sceneimage(fileName);

		esLandmarks = tr.landmarksAutoDectect(sceneimage, Degree, 500, 400, 500,
			ePoint, angleDiff);
		if (savefolder != NULL || savefolder != "")
		{
			string saveFile = savefolder.toStdString() + "/" + fileNames.at(i)
				+ ".TPS";
			ofstream inFile(saveFile.c_str());
			inFile << "LM=" << esLandmarks.size() << "\n";
			for (size_t k = 0; k < esLandmarks.size(); k++)
			{
				pk = esLandmarks.at(k);
				inFile << pk.getX() << "\t" << pk.getY() << "\n";
			}
			inFile << "IMAGE=" << saveFile << "\n";
			inFile.close();
		}
	}

	msgbox.setText("Finish");
	msgbox.exec();

}
void ImageViewer::dirCentroidMeasure()
{
	cout << "\n Compute centroid on directory." << endl;

	QMessageBox qmessage;
	qmessage.setText("Select the landmarks folder.");
	qmessage.exec();

	QString lmfolder = QFileDialog::getExistingDirectory(this);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), ".",
		tr("Measure file (*.txt)"));

	vector<string> fileNames = readDirectory(lmfolder.toStdString().c_str());
	string saveFile;
	ofstream inFile;
	if (fileName != NULL || fileName != "")
	{
		string saveFile = fileName.toStdString();
		inFile.open(saveFile.c_str(), std::ofstream::out);
	}
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		string filePath = lmfolder.toStdString() + "/" + fileNames.at(i);
		//vector<Point> mLandmarks = readTPSFile(filePath.c_str());

		//matImage->setMLandmarks(filePath);
		vector<Point> mLandmarks = matImage->readManualLandmarks(filePath);
		if (mLandmarks.size() > 0)
		{
			Point ebary(0, 0);
			double mCentroid = 0;
			mCentroid = measureCentroidPoint(mLandmarks, ebary);

			if (fileName != NULL || fileName != "")
			{
				inFile << fileNames.at(i) << "\t" << ebary.getX() << "\t"
					<< ebary.getY() << "\t" << mCentroid << "\n";

			}
		}
		else
		{
			if (fileName != NULL || fileName != "")
			{
				inFile << fileNames.at(i) << "\t" << 0 << "\t" << 0 << "\t" << 0
					<< "\n";

			}
		}
		mLandmarks.clear();
	}
	inFile.close();

	qmessage.setText("Finish.");
	qmessage.exec();
}
// generation landmarks with PHT on lines
/*void ImageViewer::dirGenerateData()
 {
 cout << "\n Automatic generate data on directory." << endl;
 QMessageBox msgbox;

 string imageFolder = "/home/linh/Desktop/Temps/md/images";
 string lmFolder = "/home/linh/Desktop/Temps/md/landmarks";
 string saveFolder = "/home/linh/Desktop/results/2017/md/random";
 vector<string> images = readDirectory(imageFolder.c_str());
 vector<string> lms = readDirectory(lmFolder.c_str());
 int nrandom = 0;
 string model;
 string sceneName;
 string lmFile;

 Point pk;
 vector<Point> esLandmarks;
 Point ePoint(0, 0);
 double angleDiff = 0;
 LandmarkDetection tr;
 for (int i = 0; i < 21; i++)
 { // run on 20 images
 nrandom = random(0, (int) images.size());

 string modelName = images.at(nrandom);
 cout << "\n Random and model: " << nrandom << "\t" << modelName << endl;
 model = imageFolder + "/" + images.at(nrandom);
 lmFile = lmFolder + "/" + lms.at(nrandom);
 matImage = new Image(model);
 matImage->readManualLandmarks(lmFile);
 tr.setRefImage(*matImage);
 tr.landmarksOnDir(modelName, imageFolder, images, Degree, 500, 400, 500,
 ePoint, angleDiff, saveFolder);
 }

 msgbox.setText("Finish");
 msgbox.exec();
 }*/
// GHT with points
void ImageViewer::dirGenerateData()
{
	cout << "\n Automatic generate data on directory." << endl;
	QMessageBox msgbox;

	string imageFolder = "/home/linh/Desktop/editedImages/md_images/size1";
	//string imageFolder = "/home/linh/Desktop/rotatedImages/mg";
	string lmFolder = "/home/linh/Desktop/editedImages/md_landmarks/size1";
	string saveFolder = "/home/linh/Desktop/results/2017/md/6mars";
	vector<string> images = readDirectory(imageFolder.c_str());
	vector<string> lms = readDirectory(lmFolder.c_str());
	int nrandom = 0;
	string model;
	string sceneName;
	string lmFile;

	Point pk;
	vector<Point> esLandmarks;
	Point ePoint(0, 0);
	LandmarkDetection tr;
	//for (int i = 0; i < 21; i++)
	//{ // run on 20 images
	//nrandom = random(0, (int) images.size());
	nrandom = 41; // use Md 028 as ref
	string modelName = images.at(nrandom);
	cout << "\n Random and model: " << nrandom << "\t" << modelName << endl;
	model = imageFolder + "/" + images.at(nrandom);
	lmFile = lmFolder + "/" + lms.at(nrandom);
	matImage = new Image(model);
	matImage->readManualLandmarks(lmFile);
	tr.setRefImage(*matImage);
	//tr.landmarksOnDir2(modelName, imageFolder, images, saveFolder);
	tr.landmarksOnDir4(modelName, imageFolder, images, saveFolder, lmFolder, lms);
	//}

	msgbox.setText("Finish");
	msgbox.exec();
}
// ================================================ Registration ========================================
void ImageViewer::pcaiMethodViewer()
{
	/*
	 * QMessageBox msgbox;

	 // extract list of points of model image by using canny
	 msgbox.setText("Select the model image.");
	 msgbox.exec();
	 QString fileName2 = QFileDialog::getOpenFileName(this);
	 if (fileName2.isEmpty())
	 return;
	 cout << endl << fileName2.toStdString() << endl;
	 Image *modelImage = new Image(fileName2.toStdString());

	 msgbox.setText("Select the landmark file of model image.");
	 msgbox.exec();
	 QString reflmPath = QFileDialog::getOpenFileName(this);
	 modelImage->readManualLandmarks(reflmPath.toStdString());
	 int rows = matImage->getGrayMatrix()->getRows();
	 int cols = matImage->getGrayMatrix()->getCols();
	 ProHoughTransform tr;
	 tr.setRefImage(*modelImage);

	 Point ePoint, mPoint;
	 double angleDiff;

	 //==================================================================
	 LandmarkDetection lmDetect;
	 lmDetect.setRefImage(*modelImage);
	 vector<Point> estLandmarks = lmDetect.landmarksAutoDectect2(*matImage, 100,
	 300);
	 cout << "\nNumber of the landmarks: " << estLandmarks.size() << endl;
	 */
	cout << "\nImage registration" << endl;
	QMessageBox msgbox;

	// extract list of points of model image by using canny
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());
	//string lmfile = "/media/linh/Data/PhD/Dataset/mg/landmarks/Mg 025.TPS";
	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	modelImage->readManualLandmarks(reflmPath.toStdString());
	vector<Point> estLandmarks = PCAI(*modelImage, *matImage,
		modelImage->getListOfManualLandmarks());
	cout << "\nNumber of the landmarks: " << estLandmarks.size() << endl;

	RGB color;
	color.R = 255;
	// display the result
	Point lm;
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::yellow);
	qpainter.setBrush(Qt::yellow);
	qpainter.setFont(QFont("Arial", 20));
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.getX() << "\t" << lm.getY()
			<< endl;
		fillCircle(*(matImage->getRGBMatrix()), lm, 5, color);
		qpainter.drawEllipse(lm.getX(), lm.getY(), 4, 4);
		qpainter.drawText(lm.getX() + 6, lm.getY(), QString::number((int) i));
	}
	qpainter.end();
	Point ebary;
	double mCentroid = measureCentroidPoint(estLandmarks, ebary);
	msgbox.setText(
		"<p>Coordinate of bary point: (" + QString::number(ebary.getX()) + ", "
			+ QString::number(ebary.getY()) + ")</p>"
				"<p>Centroid value: " + QString::number(mCentroid) + "</p");
	msgbox.exec();

	// working on folder
	/*string imageFolder = "/home/linh/Desktop/editedImages/md_images/s2tos1";
	 string saveFolder = "/media/linh/Data/PhD/Dataset/mg/save";
	 vector<string> images = readDirectory(imageFolder.c_str());
	 pcaiFolder(imageFolder, images, *matImage,
	 modelImage->getListOfManualLandmarks(),saveFolder);*/
	saveRGB("result.jpg", matImage->getRGBMatrix());
	this->loadImage(matImage, qImage, "PCAI result");
	this->show();
	//delete newScene;
	delete modelImage;
	msgbox.setText("Finish.");
	msgbox.exec();
}
