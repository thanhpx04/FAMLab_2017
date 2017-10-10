#ifndef DRAFTVIEWER_H
#define DRAFTVIEWER_H

#include "../imageModel/Point.h"
#include <QtGui/qlabel.h>
#include <QtGui/qscrollarea.h>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include "ImageLabel.h"

class Image;

class DraftViewer: public QMainWindow
{
    Q_OBJECT
public:
    DraftViewer();
    ~DraftViewer();

    QString fileName;
    QImage qImage;
    Image *matImage;

    QAction *parameterAction;
    QDialog *parameterDialog;

    void loadImage(QString fn);
    void loadImage(Image *_matImage, QImage _qImage, QString tt);
private slots:
    void about();
    void testMethod();

    void open();
    void save();
    void saveAs();

    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void gScaleHistogram();
    void rgbHistogramCalc();
    void displayManualLandmarks();
    void displayAutoLandmarks();

    void binThreshold();
    void cannyAlgorithm();
    void suzukiAlgorithm();
    void lineSegmentation();

    void gauFilter();
    void robertFilter();
    void sobelFilter();
    void erosionOperation();
    void dilationOperation();
    void openOperation();
    void closeOperation();

    //void pHoughTransform();
    void gHoughTransform();
    void extractLandmarks();
    void measureMBary();
    void measureEBary();
    void dirAutoLandmarks();
    void dirCentroidMeasure();
    void dirGenerateData();
    void sobelAndSIFT();
    void cannyAndSIFT();
    void pcaiMethodViewer();

    //Thanh
    void extractObject(int x, int y);
    void process4Quarters();
    void openFragmentScreen();

private:

    ImageLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;

    // menu
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *segmentationMenu;
    QMenu *processMenu;
    QMenu *dominantPointMenu;
    QMenu *helpMenu;
    //Thanh
    QMenu *pluginMenu;
    //============

    // toolbar
    QToolBar *fileToolBar;
    QToolBar *viewToolBar;

    //menu action
    QAction *openAct;
    QAction *printAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *exitAct;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *gHistogramAct;
    QAction *rgbHistogramAct;
    QAction *displayMLandmarksAct;
    QAction *displayALandmarksAct;

    QAction *aboutAct;
    QAction *testAct;

    QAction *binaryThresholdAct;
    QAction *cannyAct;
    QAction *suzukiAct;
    QAction *lineSegmentationAct;

    //Thanh
//    QAction *detectObjectAct;
    QAction *process4QuartersAct;
    QAction *openFragmentScreenAct;
    //==============

    QAction *gauAct;
    QAction *robertAct;
    QAction *sobelAct;
    QAction *erosionAct;
    QAction *dilationAct;
    QAction *openBinaryAct;
    QAction *closeBinaryAct;

    //QAction *phtAct;
    QAction *phtPointsAct;
    QAction *autoLandmarksAct;
    QAction *measureMBaryAct;
    QAction *measureEBaryAct;
    QAction *dirAutoLandmarksAct;
    QAction *dirCentroidMeasureAct;
    QAction *dirGenerateDataAct;
    QAction *sobelAndSIFTAct;
    QAction *cannyAndSIFTAct;

    // Action registration
    QAction *pcaiAct;

    // private methods
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createFileMenu();
    void createViewMenu();
    void createHelpMenu();
    void createSegmentationMenu();
    void createLandmarksMenu();
    void createRegistrationMenu();
    void createFilterMenu();
    //Thanh
    void createPluginMenu();
    //=============
    void activeFunction();
    void viewMenuUpdateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void displayLandmarks(Image *image, std::vector<Point> lms, RGB color);
};

#endif // DRAFTVIEWER_H
