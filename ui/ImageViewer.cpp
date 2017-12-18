#include "ImageViewer.h"

ImageViewer::ImageViewer()
{
    matImage = new Image;
    imageLabel = new ImageLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createFileMenu();

    createMenus();
    createToolBars();
    createStatusBar();

    setWindowTitle(tr(".: Image viewer :."));
    resize(900, 700);

    setWindowIcon(QIcon("./resources/ico/ip.ico"));

    connect(this->imageLabel,SIGNAL(sendMouseLocation(int,int)),this,SLOT(extractObject(int,int)));
}

ImageViewer::~ImageViewer()
{
    delete matImage;
    delete imageLabel;
    delete scrollArea;

    // menu
    delete fileMenu;

    // toolbar
    delete fileToolBar;

    //menu action
    delete openAct;
}

void ImageViewer::createFileMenu()
{
    openAct = new QAction(QIcon("./resources/ico/open.png"), tr("&Open..."),
                          this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
}

void ImageViewer::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    // add menus to GUI
    menuBar()->addMenu(fileMenu);
}

void ImageViewer::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
}

void ImageViewer::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void ImageViewer::loadImage(QString fn)
{
    matImage = new Image(fn.toStdString());
    qImage.load(fn);

    imageLabel->setPixmap(QPixmap::fromImage(qImage));
    imageLabel->adjustSize();

    this->fileName = fn;
    setWindowTitle(tr("Image Viewer - ") + fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void ImageViewer::loadImage(Image *_matImage, QImage _qImage, QString tt)
{
    matImage = _matImage;
    qImage = _qImage;
    imageLabel->setPixmap(QPixmap::fromImage(qImage));
    imageLabel->adjustSize();

    setWindowTitle(tr("Image Viewer - ") + tt);
    statusBar()->showMessage(tr("Finished"), 2000);
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
        this->loadImage(fileName);
    }
}

/**
  Extract chosen object with the coordinates from mouse event double click.
  The algorithm is region growing on gray matrix using threshold value for growing criteria.

  @param the coordinates of mouse click.
*/
void ImageViewer::extractObject(int x, int y)
{
    cout << "Extract object from the point: "<<x<<"  "<<y << endl;

    // compute threshold value
    float tValue = matImage->getThresholdValue();
//    Segmentation tr;
//    tr.setRefImage(*matImage);
    cout << endl << "tValue: " << tValue << endl;

    // obtain grayMatrix of image to detect chosen object
    int rows = matImage->getRGBMatrix()->getRows();
    int cols = matImage->getRGBMatrix()->getCols();
    ptr_IntMatrix grayMatrix = new Matrix<int>(rows, cols);
    grayMatrix->setData(matImage->getGrayMatrix()->getData());

    int minX=x, maxX=x, minY=y, maxY=y; // to determine the rectangle of object

    // apply region growing to obtain the grayMatrix containing object which is labeled
    Point startingPoint(x,y);
    detectRegion(grayMatrix,tValue,startingPoint,minX,maxX,minY,maxY);

    // initial object Matrix
    // canny algorithm in MARLab didn't compute the edge of object which meet the border
    // therefore I add 2 pixels more in each side to obtain the full border of object
    int rowsOfObject = maxY-minY+4;
    int colsOfObject = maxX-minX+4;
    RGBA defaultValue;
    defaultValue.A = 0;
    defaultValue.R = 0;
    defaultValue.G = 0;
    defaultValue.B = 0;
    ptrRGBAMatrix objectRGBAMatrix = new Matrix<RGBA>(rowsOfObject, colsOfObject, defaultValue);
    ptr_IntMatrix objectBinMatrix = new Matrix<int>(rowsOfObject, colsOfObject, 255);

    // obtain the color value of labeled pixels
    ptr_RGBMatrix srcMatrix = matImage->getRGBMatrix();
    for (int r = 2; r < rowsOfObject; r++)
    {
        for (int c = 2; c < colsOfObject; c++)
        {
            int grayValue = grayMatrix->getAtPosition(r-2+minY,c-2+minX);
            // check labeled pixels of object
            if(grayValue == 256)
            {
                RGB currentColor = srcMatrix->getAtPosition(r-2+minY,c-2+minX);
                RGBA currentColorWithAlpha;
                currentColorWithAlpha.A = 255;// set transparency
                currentColorWithAlpha.R = currentColor.R;
                currentColorWithAlpha.G = currentColor.G;
                currentColorWithAlpha.B = currentColor.B;
                objectRGBAMatrix->setAtPosition(r,c,currentColorWithAlpha);
                objectBinMatrix->setAtPosition(r,c,0);
            }
        }
    }

    // apply canny algorithm to detect boundary and suzuky to get list edges to draw it
    vector<Point> cPoints;
    ptr_IntMatrix cannyMatrix = cannyProcess(objectBinMatrix, (int) tValue,
                                             3 * (int) tValue, cPoints);
    vector<Edge> listOfEdges = suzuki(cannyMatrix);

    //find the longest Edge
    Edge longestEdge = listOfEdges.at(0);
    for(size_t i =1; i<listOfEdges.size();i++)
    {
        Edge current = listOfEdges.at(i);
        if(longestEdge.getPoints().size()< current.getPoints().size())
            longestEdge = current;
    }

    // paint red line
    RGBA red;
    red.R = red.A = 255;
    red.G = red.B = 0;
    rows = objectRGBAMatrix->getRows();
    cols = objectRGBAMatrix->getCols();
//    vector<Point> drawPoints = cPoints;
    vector<Point> drawPoints = longestEdge.getPoints();
    for (size_t k = 0; k < drawPoints.size(); k++)
    {
        Point pi = drawPoints.at(k);
        if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
                && pi.getY() < rows)
        {
            objectRGBAMatrix->setAtPosition(pi.getY(), pi.getX(), red);
        }
    }

//    Edge border;
//    border.setPoints(cPoints);
//    Point pointMinX = findMinPointOfBorder(border);


    // emmit to signal in order to send to matching fragment window
    string imageName = matImage->getFileName();
    emit sendObjectRGBA(imageName, minX, minY, objectRGBAMatrix, longestEdge);
    cout << "send object" << endl;
}
