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
#include "../segmentation/Filters.h"

#include "../pht/GHTInPoint.h"
#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../pht/PCA.h"
#include "../pht/SVD.h"

#include "../histograms/ShapeHistogram.h"
#include "../histograms/LBP.h"
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

#include "DraftViewer.h"
#include "TestWindow.h"

DraftViewer::DraftViewer()
{
    imageLabel = new ImageLabel;
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

    connect(this->imageLabel,SIGNAL(sendMouseLocation(int,int)),this,SLOT(extractObject(int,int)));
}

DraftViewer::~DraftViewer()
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
    //Thanh
    delete pluginMenu;
    //========

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
    //Thanh
    //    delete detectObjectAct;
    delete process4QuaterAct;
    delete openFragmentScreenAct;
    //==========

    delete gauAct;
    delete robertAct;
    delete sobelAct;
    delete dilationAct;
    delete erosionAct;
    delete openBinaryAct;
    delete closeBinaryAct;

    //delete phtAct;
    delete phtPointsAct;
    delete autoLandmarksAct;
    delete measureMBaryAct;
    delete measureEBaryAct;
    delete dirAutoLandmarksAct;
    delete dirCentroidMeasureAct;
    delete dirGenerateDataAct;
    delete sobelAndSIFTAct;
    delete cannyAndSIFTAct;
}

void DraftViewer::loadImage(QString fn)
{
    //test call PNG decompress
    //    string filePath = fn.toStdString();
    //    int rows = 0, cols = 0;
    //    cout << filePath << endl;
    //    cout << filePath.c_str() << endl;
    //    ptrRGBAMatrix test = readPNGToRGBA(filePath.c_str(), rows, cols);

    matImage = new Image(fn.toStdString());
    qImage.load(fn);

    imageLabel->setPixmap(QPixmap::fromImage(qImage));
    scaleFactor = 1.0;

    saveAsAct->setEnabled(true);
    activeFunction();

    this->fileName = fn;
    setWindowTitle(tr("Draft Viewer - ") + fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void DraftViewer::loadImage(Image *_matImage, QImage _qImage, QString tt)
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

// =========================== Slots =======================================

void DraftViewer::about()
{
    QMessageBox::about(this, tr("About MAELab"),
                       tr(
                           "<p><b>MAELab</b> is a software in Computer Vision. It provides the function to "
                           "segmentation and detection dominant points.</p>"));
}

void DraftViewer::testMethod()
{
    cout << "\nTest a method ..." << endl;
    RGB df;
    df.R = df.G = df.B = 0;
    Matrix<int> patch = matImage->getGrayMatrix()->extractPatch(501,501,676,2746,0);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(&patch),
                     "Test a method");
    other->move(x() - 40, y() - 40);
    other->show();

    cout << "\nFinish.\n";
    cout << "\n End test method!" << endl;
}

void DraftViewer::open()
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
            DraftViewer* other = new DraftViewer;
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

void DraftViewer::save()
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

void DraftViewer::saveAs()
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

void DraftViewer::zoomIn()
{
    scaleImage(1.25);
}

void DraftViewer::zoomOut()
{
    scaleImage(0.8);
}

void DraftViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void DraftViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
    {
        normalSize();
    }
    viewMenuUpdateActions();
}

void DraftViewer::gScaleHistogram()
{
    cout << "\n Gray scale histogram." << endl;
    ptr_IntMatrix histogram = matImage->getGrayHistogram();
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
        pend.setY(239 - (histogram->getAtPosition(0, c) * 239 / max));
        drawingLine(*hDisplay, Line(pbegin, pend), color);
    }
    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrRGBToQImage(hDisplay), "Histogram result");
    other->move(x() - 40, y() - 40);
    other->show();
    cout << "\nFinished." << endl;
}

void DraftViewer::rgbHistogramCalc()
{
    cout << "\n RGB histogram." << endl;

    ptr_RGBMatrix histogram = matImage->getRGBHistogram();
    ptr_RGBMatrix result = colorThreshold(matImage->getRGBMatrix(), histogram);

    double maxR = -1, maxG = -1, maxB = -1;
    for (int c = 0; c < histogram->getCols(); c++)
    {
        RGB color = histogram->getAtPosition(0, c);
        if (color.R > maxR)
            maxR = color.R;
        if (color.G > maxG)
            maxG = color.G;
        if (color.B > maxB)
            maxB = color.B;
    }
    int cols = histogram->getCols();
    RGB color;
    color.R = color.G = color.B = 0;
    ptr_RGBMatrix redDisplay = new Matrix<RGB>(240, 300, color);
    ptr_RGBMatrix greenDisplay = new Matrix<RGB>(240, 300, color);
    ptr_RGBMatrix blueDisplay = new Matrix<RGB>(240, 300, color);
    Point pbegin, pend;
    for (int c = 0; c < histogram->getCols(); c++)
    {
        RGB cvalue = histogram->getAtPosition(0, c);
        pbegin.setX(c);
        pbegin.setY(239);
        pend.setX(c);
        pend.setY(239 - ((double) ((cvalue.R * 239) / maxR)));
        color.R = 255;
        color.G = color.B = 0;
        drawingLine(*redDisplay, Line(pbegin, pend), color);

        pend.setX(c);
        pend.setY(239 - ((double) ((cvalue.G * 239) / maxG)));
        color.G = 255;
        color.R = color.B = 0;
        drawingLine(*greenDisplay, Line(pbegin, pend), color);

        pend.setX(c);
        pend.setY(239 - ((double) ((cvalue.B * 239) / maxB)));
        color.B = 255;
        color.R = color.G = 0;
        drawingLine(*blueDisplay, Line(pbegin, pend), color);
    }

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrRGBToQImage(result),
                     "RGB Histogram result - Red channel");
    other->move(x() - 40, y() - 40);
    other->show();

    /*ImageViewer *other2 = new ImageViewer;
     other2->loadImage(matImage, ptrRGBToQImage(greenDisplay),
     "RGB Histogram result - Green channel");
     other2->move(x() - 60, y() - 60);
     other2->show();

     ImageViewer *other3 = new ImageViewer;
     other3->loadImage(matImage, ptrRGBToQImage(blueDisplay),
     "RGB Histogram result - Blue Channel");
     other3->move(x() - 80, y() - 80);
     other3->show();*/

    cout << "\nFinished." << endl;
}

void DraftViewer::displayManualLandmarks()
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

void DraftViewer::displayAutoLandmarks()
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

void DraftViewer::binThreshold()
{
    cout << "\nBinary thresholding...\n";
    float tValue = matImage->getThresholdValue();

    Segmentation tr; // = new Segmentation();
    tr.setRefImage(*matImage);
    cout << "\ntValue: " << tValue << endl;
    ptr_IntMatrix rsMatrix = tr.threshold(tValue, 255);

    // Thanh: ignore postProcess function which is filling the hole
    //    rsMatrix = postProcess(rsMatrix, 255);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(rsMatrix), "Thresholding result");
    other->show();
}

void DraftViewer::cannyAlgorithm()
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
        cout << "\nNumber of points in edge: " << edgei.getPoints().size();
    }
    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
                     "Canny result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::suzukiAlgorithm()
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
    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
                     "Canny result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::lineSegmentation()
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
                    "Line segmentation result");
    this->show();

    msgbox.setText("Finish");
    msgbox.exec();
}

void DraftViewer::gauFilter()
{
    cout << "\n Gaussian filter." << endl;
    Matrix<double> kernel = getGaussianKernel(3, 1);
    Matrix<int> gsResult = gaussianBlur(*(matImage->getGrayMatrix()), kernel);
    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(&gsResult),
                     "Gaussian filter result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::robertFilter()
{
    cout << "\n Robert filter." << endl;
    Matrix<int> rbResult = RobertOperation(matImage->getGrayMatrix());
    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(&rbResult), "Robert filter result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::sobelFilter()
{
    cout << "\n Sobel filter." << endl;
    Matrix<int> sbResult = SobelOperation(matImage->getGrayMatrix());
    sbResult = postSobel(sbResult);
    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(&sbResult), "Sobel filter result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::erosionOperation()
{
    cout << "\n Erosion operation." << endl;
    Matrix<int> sbResult = SobelOperation(matImage->getGrayMatrix());
    sbResult = postSobel(sbResult);

    ptr_IntMatrix erResult = erode(&sbResult, 3);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(erResult),
                     "Erosion operation result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::dilationOperation()
{
    cout << "\n Dilation operation." << endl;
    Matrix<int> sbResult = SobelOperation(matImage->getGrayMatrix());
    sbResult = postSobel(sbResult);

    ptr_IntMatrix dlResult = dilate(&sbResult, 3);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(dlResult),
                     "Dilation operation result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::openOperation()
{
    cout << "\n Open operation." << endl;
    Matrix<int> sbResult = SobelOperation(matImage->getGrayMatrix());
    sbResult = postSobel(sbResult);

    ptr_IntMatrix dlResult = openBinary(&sbResult, 3);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(dlResult), "Open operation result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::closeOperation()
{
    cout << "\n Close operation." << endl;
    Matrix<int> sbResult = SobelOperation(matImage->getGrayMatrix());
    sbResult = postSobel(sbResult);

    ptr_IntMatrix dlResult = closeBinary(&sbResult, 3);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(dlResult),
                     "Close operation result");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::gHoughTransform()
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

void DraftViewer::extractLandmarks()
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

void DraftViewer::measureMBary()
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

void DraftViewer::measureEBary()
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

void DraftViewer::dirAutoLandmarks()
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

void DraftViewer::dirCentroidMeasure()
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

void DraftViewer::dirGenerateData()
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

void DraftViewer::sobelAndSIFT()
{
    cout << "\nLandmarks with contours (Sobel) and SIFT" << endl;
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

    // get the contours in the scene image
    Matrix<double> kernel = getGaussianKernel(5, 2);
    Matrix<int> gsResult = gaussianBlur(*(matImage->getGrayMatrix()), kernel);
    Matrix<int> sbResult = SobelOperation(&gsResult);
    int tValue = 6; //thresholdOtsu(sbResult);
    cout << "\n Otsu threshold: " << tValue << endl;
    vector<Point> contours;
    for (int r = 0; r < sbResult.getRows(); r++)
    {
        for (int c = 0; c < sbResult.getCols(); c++)
        {
            int value = sbResult.getAtPosition(r, c);
            if (value == 0)
            {
                sbResult.setAtPosition(r, c, 0);
            }
            else
            {
                if (value >= tValue)
                {
                    sbResult.setAtPosition(r, c, 255);
                }
                else
                    sbResult.setAtPosition(r, c, 0);
            }
        }
    }
    ptr_IntMatrix dlResult = closeBinary(&sbResult, 3);
    for (int r = 0; r < dlResult->getRows(); r++)
    {
        for (int c = 0; c < dlResult->getCols(); c++)
        {
            if (dlResult->getAtPosition(r, c) == 255)
                contours.push_back(Point(c, r));
        }
    }
    cout << "\nManual landmarks: "
         << modelImage->getListOfManualLandmarks().size() << endl;
    vector<Point> estLandmarks = verifyDescriptors3(modelImage->getGrayMatrix(),
                                                    matImage->getGrayMatrix(), contours, modelImage->getListOfManualLandmarks(),
                                                    9);
    cout << "\nNumber of estimated landmarks: " << estLandmarks.size() << endl;

    RGB color;
    color.R = 255;
    color.G = 0;
    color.B = 0;
    Point lm;
    for (size_t i = 0; i < estLandmarks.size(); i++)
    {
        lm = estLandmarks.at(i);
        fillCircle(*(matImage->getRGBMatrix()), lm, 7, color);
    }

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
                     "Sobel and SIFT");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::cannyAndSIFT()
{
    cout << "\nLandmarks with contours (Sobel) and SIFT" << endl;
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

    vector<Point> cPoints;
    matImage->cannyAlgorithm(cPoints);
    cout << "\nNumber of points on contours: " << cPoints.size() << endl;
    vector<Point> estLandmarks = verifyDescriptors4(modelImage->getGrayMatrix(),
                                                    matImage->getGrayMatrix(), cPoints, modelImage->getListOfManualLandmarks(),
                                                    63);
    cout << "\nNumber of estimated landmarks: " << estLandmarks.size() << endl;
    RGB color;
    color.R = 255;
    color.G = 0;
    color.B = 0;
    Point lm;
    for (size_t i = 0; i < estLandmarks.size(); i++)
    {
        lm = estLandmarks.at(i);
        fillCircle(*(matImage->getRGBMatrix()), lm, 7, color);
    }

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrRGBToQImage(matImage->getRGBMatrix()),
                     "Canny and SIFT");
    other->move(x() - 40, y() - 40);
    other->show();
}

void DraftViewer::pcaiMethodViewer()
{
    cout << "\nLandmarks with image registration" << endl;
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

/**
 * Thanh
 * Extract chosen object with the coordinates from mouse event double click.
 * The algorithm is region growing on gray matrix using threshold value for growing criteria.
 * @param the coordinates of mouse click.
*/
void DraftViewer::extractObject(int x, int y)
{
    cout << "Extract object from the point: "<<x<<"  "<<y << endl;

    float tValue = matImage->getThresholdValue();
    Segmentation tr;
    tr.setRefImage(*matImage);
    cout << endl << "tValue: " << tValue << endl;

    int rows = matImage->getRGBMatrix()->getRows();
    int cols = matImage->getRGBMatrix()->getCols();

    // copy grayMatrix of image to process
    ptr_IntMatrix grayMatrix = new Matrix<int>(rows, cols);
    grayMatrix->setData(matImage->getGrayMatrix()->getData());

    // to determine the rectangle of object
    int minX=x, maxX=x, minY=y, maxY=y;

    // list of points to be checked.
    vector<Point> seedPoints;
    RGB color;
    Point startPoint(x,y,color);
    seedPoints.push_back(startPoint);
    // label start Point is checked by value 256
    grayMatrix->setAtPosition(y,x,256);
    while(!seedPoints.empty())
    {
        // Pop a point from seedPoints (remove it from the list)
        Point checkingPoint = seedPoints.back();
        seedPoints.pop_back();

        vector<Point> tempList = tr.growRegion(grayMatrix,tValue,checkingPoint,minX,maxX,minY,maxY);
        seedPoints.insert(seedPoints.end(),tempList.begin(),tempList.end());
    }

    // fix the rectangel of object
    rows = maxY-minY+4;
    cols = maxX-minX+4;
    ptr_RGBMatrix srcMatrix = matImage->getRGBMatrix();
    // set the background is black => it can be use alpha channel
    ptr_RGBMatrix objectRGBMatrix = new Matrix<RGB>(rows, cols);
    ptr_IntMatrix objectBinMatrix = new Matrix<int>(rows, cols, 255);
    for (int r = 2; r < rows; r++)
    {
        for (int c = 2; c < cols; c++)
        {
            int grayValue = grayMatrix->getAtPosition(r-2+minY,c-2+minX);
            // check position of object only
            if(grayValue == 256)
            {
                objectRGBMatrix->setAtPosition(r,c,srcMatrix->getAtPosition(r-2+minY,c-2+minX));
                objectBinMatrix->setAtPosition(r,c,0);
            }
        }
    }

    // implement canny to detect boundary and suzuky to get list edges to draw it
    vector<Point> cPoints;
    ptr_IntMatrix cannyMatrix = cannyProcess(objectBinMatrix, (int) tValue,
                                             3 * (int) tValue, cPoints);

    vector<Edge> listOfEdges;
    listOfEdges = suzuki(cannyMatrix);

    for (size_t i = 0; i < listOfEdges.size(); i++)
    {
        Edge edgei = listOfEdges.at(i);
        for (size_t k = 0; k < edgei.getPoints().size(); k++)
        {
            Point pi = edgei.getPoints().at(k);
            if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
                    && pi.getY() < rows)
            {
                RGB red;
                red.R = 255;
                red.G = red.B = 0;
                objectRGBMatrix->setAtPosition(pi.getY(), pi.getX(), red);
            }
        }
        //cout << "Number of points in edge: " << edgei.getPoints().size() << endl;
    }

    DraftViewer *other1 = new DraftViewer;
    other1->loadImage(matImage, ptrRGBToQImage(objectRGBMatrix), "Object color result");
    other1->show();
}

void DraftViewer::process4Quater()
{
    Segmentation tr;
    tr.setRefImage(*matImage);

    // declare
    ptr_IntMatrix grayHistogram;
    float medianHistogram;
    float meanHistogram;
    float thresholdValue;
    ptr_IntMatrix aQuaterGrayMatrix;

    int rows = matImage->getRGBMatrix()->getRows();
    int cols = matImage->getRGBMatrix()->getCols();
    int halfRows = rows/2;
    int halfCols = cols/2;

    // compute on 1st quater
    aQuaterGrayMatrix = copyGrayMatrix(matImage->getGrayMatrix(),0,halfRows,0,halfCols);
    grayHistogram = calculateHistogram(aQuaterGrayMatrix, medianHistogram, meanHistogram, thresholdValue);
    thresholdValue = calculateMAELabThreshold(medianHistogram,meanHistogram,grayHistogram);
    cout << "1st Quater threshold value: " << thresholdValue << endl;
    ptr_IntMatrix firstQuaterBinaryMatrix = binaryThreshold(aQuaterGrayMatrix,thresholdValue,255);

    // compute on 2nd quater
    aQuaterGrayMatrix = copyGrayMatrix(matImage->getGrayMatrix(),0,halfRows,halfCols,cols);
    grayHistogram = calculateHistogram(aQuaterGrayMatrix, medianHistogram, meanHistogram, thresholdValue);
    thresholdValue = calculateMAELabThreshold(medianHistogram,meanHistogram,grayHistogram);
    cout << "2nd Quater threshold value: " << thresholdValue << endl;
    ptr_IntMatrix secondQuaterBinaryMatrix = binaryThreshold(aQuaterGrayMatrix,thresholdValue,255);

    // compute on 3rd quater
    aQuaterGrayMatrix = copyGrayMatrix(matImage->getGrayMatrix(),halfRows,rows,0,halfCols);
    grayHistogram = calculateHistogram(aQuaterGrayMatrix, medianHistogram, meanHistogram, thresholdValue);
    thresholdValue = calculateMAELabThreshold(medianHistogram,meanHistogram,grayHistogram);
    cout << "3rd Quater threshold value: " << thresholdValue << endl;
    ptr_IntMatrix thirdQuaterBinaryMatrix = binaryThreshold(aQuaterGrayMatrix,thresholdValue,255);

    // compute on 4th quater
    aQuaterGrayMatrix = copyGrayMatrix(matImage->getGrayMatrix(),halfRows,rows,halfCols,cols);
    grayHistogram = calculateHistogram(aQuaterGrayMatrix, medianHistogram, meanHistogram, thresholdValue);
    thresholdValue = calculateMAELabThreshold(medianHistogram,meanHistogram,grayHistogram);
    cout << "4th Quater threshold value: " << thresholdValue << endl;
    ptr_IntMatrix fourthQuaterBinaryMatrix = binaryThreshold(aQuaterGrayMatrix,thresholdValue,255);

    // copy 1st quater to imageGray
    ptr_IntMatrix binaryImageMatrix = new Matrix<int>(rows,cols);
    copySmallToBigIntMatrix(firstQuaterBinaryMatrix,binaryImageMatrix,0,0);
    copySmallToBigIntMatrix(secondQuaterBinaryMatrix,binaryImageMatrix,0,halfCols);
    copySmallToBigIntMatrix(thirdQuaterBinaryMatrix,binaryImageMatrix,halfRows,0);
    copySmallToBigIntMatrix(fourthQuaterBinaryMatrix,binaryImageMatrix,halfRows,halfCols);

    DraftViewer *other = new DraftViewer;
    other->loadImage(matImage, ptrIntToQImage(binaryImageMatrix), "4 Quater");
    other->show();
}

void DraftViewer::openFragmentScreen()
{
    TestWindow *tw = new TestWindow;
    tw->show();
}
//================================================================================

void DraftViewer::createActions()
{
    createFileMenu();
    createViewMenu();
    createHelpMenu();
    createSegmentationMenu();
    createLandmarksMenu();
    createFilterMenu();
    //Thanh
    createPluginMenu();
}

void DraftViewer::createMenus()
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
    viewMenu->addAction(rgbHistogramAct);
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

    processMenu = new QMenu(tr("&Process"), this);
    QMenu* filterMenu = processMenu->addMenu(tr("Filter"));
    filterMenu->addAction(gauAct);
    filterMenu->addAction(robertAct);
    filterMenu->addAction(sobelAct);
    QMenu* binaryMenu = processMenu->addMenu(tr("Binary"));
    binaryMenu->addAction(erosionAct);
    binaryMenu->addAction(dilationAct);
    binaryMenu->addAction(openBinaryAct);
    binaryMenu->addAction(closeBinaryAct);

    dominantPointMenu = new QMenu(tr("&Landmarks"), this);
    //dominantPointMenu->addAction(phtAct);
    dominantPointMenu->addAction(phtPointsAct);
    dominantPointMenu->addAction(autoLandmarksAct);
    dominantPointMenu->addAction(pcaiAct);
    dominantPointMenu->addAction(sobelAndSIFTAct);
    dominantPointMenu->addAction(cannyAndSIFTAct);
    dominantPointMenu->addSeparator();
    dominantPointMenu->addAction(measureMBaryAct);
    dominantPointMenu->addAction(measureEBaryAct);
    dominantPointMenu->addSeparator();
    QMenu* menuDirectory = dominantPointMenu->addMenu(tr("Working on directory"));
    menuDirectory->addAction(dirAutoLandmarksAct);
    menuDirectory->addAction(dirCentroidMeasureAct);
    menuDirectory->addAction(dirGenerateDataAct);

    //Thanh
    pluginMenu = new QMenu(tr("&Plugin"), this);
    QMenu* extractObjectMenu = pluginMenu->addMenu(tr("Extract Object"));
    //    extractObjectMenu->addAction(detectObjectAct);
    extractObjectMenu->addAction(process4QuaterAct);
    pluginMenu->addAction(openFragmentScreenAct);
    //===============

    //registrationMenu = new QMenu(tr("&Registration"), this);
    //registrationMenu->addAction(icpAct);

    // add menus to GUI
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(segmentationMenu);
    menuBar()->addMenu(processMenu);
    menuBar()->addMenu(dominantPointMenu);
    //menuBar()->addMenu(registrationMenu);
    menuBar()->addMenu(helpMenu);
    //Thanh
    menuBar()->addMenu(pluginMenu);
}

void DraftViewer::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);
}

void DraftViewer::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void DraftViewer::createFileMenu()
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

void DraftViewer::createViewMenu()
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

    gHistogramAct = new QAction(tr("&Gray-scale Histogram"), this);
    gHistogramAct->setEnabled(false);
    connect(gHistogramAct, SIGNAL(triggered()), this, SLOT(gScaleHistogram()));

    rgbHistogramAct = new QAction(tr("&RGB Histogram"), this);
    rgbHistogramAct->setEnabled(false);
    connect(rgbHistogramAct, SIGNAL(triggered()), this, SLOT(rgbHistogramCalc()));

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

void DraftViewer::createHelpMenu()
{
    aboutAct = new QAction(tr("&About MAELab"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    testAct = new QAction(tr("&Test a function"), this);
    testAct->setEnabled(false);
    connect(testAct, SIGNAL(triggered()), this, SLOT(testMethod()));
}

void DraftViewer::createSegmentationMenu()
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

void DraftViewer::createLandmarksMenu()
{
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

    sobelAndSIFTAct = new QAction(tr("Sobel and SIFT method"), this);
    sobelAndSIFTAct->setEnabled(false);
    //sobelAndSIFTAct->setShortcut(tr("Ctrl+I"));
    connect(sobelAndSIFTAct, SIGNAL(triggered()), this, SLOT(sobelAndSIFT()));

    cannyAndSIFTAct = new QAction(tr("Canny and SIFT method"), this);
    cannyAndSIFTAct->setEnabled(false);
    //sobelAndSIFTAct->setShortcut(tr("Ctrl+I"));
    connect(cannyAndSIFTAct, SIGNAL(triggered()), this, SLOT(cannyAndSIFT()));

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

void DraftViewer::createFilterMenu()
{
    gauAct = new QAction(tr("&Gaussian filter"), this);
    gauAct->setEnabled(false);
    connect(gauAct, SIGNAL(triggered()), this, SLOT(gauFilter()));

    robertAct = new QAction(tr("&Robert filter"), this);
    robertAct->setEnabled(false);
    connect(robertAct, SIGNAL(triggered()), this, SLOT(robertFilter()));

    sobelAct = new QAction(tr("&Sobel filter"), this);
    sobelAct->setEnabled(false);
    connect(sobelAct, SIGNAL(triggered()), this, SLOT(sobelFilter()));

    erosionAct = new QAction(tr("&Erosion"), this);
    erosionAct->setEnabled(false);
    connect(erosionAct, SIGNAL(triggered()), this, SLOT(erosionOperation()));

    dilationAct = new QAction(tr("&Dilation"), this);
    dilationAct->setEnabled(false);
    connect(dilationAct, SIGNAL(triggered()), this, SLOT(dilationOperation()));

    openBinaryAct = new QAction(tr("&Open"), this);
    openBinaryAct->setEnabled(false);
    connect(openBinaryAct, SIGNAL(triggered()), this, SLOT(openOperation()));

    closeBinaryAct = new QAction(tr("&Close"), this);
    closeBinaryAct->setEnabled(false);
    connect(closeBinaryAct, SIGNAL(triggered()), this, SLOT(closeOperation()));
}

void DraftViewer::createPluginMenu()
{
    //    detectObjectAct = new QAction(tr("&Based on Binary threshold"), this);
    //    detectObjectAct->setEnabled(false);
    //    connect(detectObjectAct, SIGNAL(triggered()), this, SLOT(detectObjects()));

    process4QuaterAct = new QAction(tr("&Process 4 Quater"), this);
    process4QuaterAct->setEnabled(false);
    connect(process4QuaterAct, SIGNAL(triggered()), this, SLOT(process4Quater()));

    openFragmentScreenAct = new QAction(tr("&Open Fragment Screen"), this);
    openFragmentScreenAct->setEnabled(true);
    connect(openFragmentScreenAct, SIGNAL(triggered()), this, SLOT(openFragmentScreen()));

}

void DraftViewer::activeFunction()
{
    openAct->setEnabled(true);
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);

    zoomInAct->setEnabled(true);
    zoomOutAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    normalSizeAct->setEnabled(true);
    gHistogramAct->setEnabled(true);
    rgbHistogramAct->setEnabled(true);
    displayMLandmarksAct->setEnabled(true);
    if (matImage->getListOfAutoLandmarks().size() > 0)
        displayALandmarksAct->setEnabled(true);

    binaryThresholdAct->setEnabled(true);
    cannyAct->setEnabled(true);
    suzukiAct->setEnabled(true);
    lineSegmentationAct->setEnabled(true);
    //Thanh
    //    detectObjectAct->setEnabled(true);
    process4QuaterAct->setEnabled(true);
    //==========

    gauAct->setEnabled(true);
    robertAct->setEnabled(true);
    sobelAct->setEnabled(true);
    erosionAct->setEnabled(true);
    dilationAct->setEnabled(true);
    openBinaryAct->setEnabled(true);
    closeBinaryAct->setEnabled(true);

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
    sobelAndSIFTAct->setEnabled(true);
    cannyAndSIFTAct->setEnabled(true);

    testAct->setEnabled(true);

    viewMenuUpdateActions();
    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void DraftViewer::viewMenuUpdateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void DraftViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void DraftViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(
                int(
                    factor * scrollBar->value()
                    + ((factor - 1) * scrollBar->pageStep() / 2)));
}

void DraftViewer::displayLandmarks(Image *image, std::vector<Point> lms, RGB color)
{
    Point lm;
    for (size_t i = 0; i < lms.size(); i++)
    {
        lm = lms.at(i);
        cout << "\nManual landmark: " << lm.getX() << "\t" << lm.getY();
        fillCircle(*(image->getRGBMatrix()), lm, 3, color);

    }
}
