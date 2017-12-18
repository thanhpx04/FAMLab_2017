#include "WindowFragments.h"

WindowFragments::WindowFragments()
{
    createActions();
    createMenus();
    createToolBars();

    scene = new SceneFragments(fragmentMenu);

    imageViewer = new ImageViewer;
    leftFragment = new FragmentItem;
    rightFragment = new FragmentItem;

    setWindowIcon(QIcon("./resources/ico/FAMLab.png"));

    connect(this->imageViewer,SIGNAL(sendObjectRGBA(string,int,int,ptrRGBAMatrix,Edge)),this,SLOT(loadObject(string,int,int,ptrRGBAMatrix,Edge)));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    view = new QGraphicsView(scene);
    mainLayout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);

    setCentralWidget(widget);
    setWindowTitle(tr(".: Papyrus FAMLab :."));
    setUnifiedTitleAndToolBarOnMac(true);
}

WindowFragments::~WindowFragments()
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
    delete propertyAction;

    delete openAction;
    delete exitAction;
    delete aboutAction;

    delete leftFragment;
    delete rightFragment;

    delete imageViewer;
}

void WindowFragments::open()
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
        imageViewer->loadImage(fileName);
        imageViewer->show();
    }
}

void WindowFragments::about()
{
    QMessageBox::about(this, tr("About FAMLab"),
                       tr(
                           "<p><b>FAMLab</b> is a software in computer vision. It provides the functions to "
                           "extraction and matching fragments.</p>"));
}

void WindowFragments::deleteItem()
{
    QGraphicsItem *selected = selectedItem();
    if(selected){
        scene->removeItem(selected);
    }
}

void WindowFragments::bringToFront()
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

void WindowFragments::sendToBack()
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

void WindowFragments::rotateleft()
{
    QGraphicsItem *selected = selectedItem();
    if(selected){
        rotateCenter(selected,-1);
    }
}

void WindowFragments::rotateright()
{
    QGraphicsItem *selected = selectedItem();
    if(selected){
        rotateCenter(selected,+1);
    }
}

void WindowFragments::setLeftFragment()
{
    leftFragment = dynamic_cast<FragmentItem *> (selectedItem());
    if(leftFragment){
        cout << "set left" << endl;
    }
}

void WindowFragments::setRightFragment()
{
    rightFragment = dynamic_cast<FragmentItem *> (selectedItem());
    if(rightFragment){
        cout << "set right" << endl;
    }
}

void WindowFragments::propertyItem()
{
    FragmentItem *selected = dynamic_cast<FragmentItem *> (selectedItem());
    if(selected){
        string imageName = selected->getName();
        int xImage = selected->getImageX();
        int yImage = selected->getImageY();

        QRectF rect;
        rect = selected->mapToScene(selected->boundingRect()).boundingRect();
        int x = rect.x();
        int y = rect.y();
        int rotation = selected->rotation();

        ostringstream result;
        result << "<p>The fragment is in location (" <<
                  xImage << ", " << yImage << ") of image: " <<
                  imageName << "</p><p>On the screen at (" <<
                  x << ", " << y << ")</p><p>Its rotation: "<<
                  rotation << "</p>";

        QMessageBox::information(this, "Information of fragment",QString::fromStdString(result.str()));
    }
}

void WindowFragments::suggestDTW()
{
    if(leftFragment && rightFragment){
        cout << "DTW" << endl;
        // this 2 list is used to draw connected lines
        vector<Point> listPointsBeginToDraw;
        vector<Point> listPointsEndToDraw;
        QVector<QGraphicsLineItem*> suggestLines;
        // --------------------------------------------

        // get list points from Left
        vector<Point> listPointsOfLeft = findMaxXmappingY(leftFragment->getBorder());
        // get every 20 pixel
        vector<Point> listPointsOfLeft20Pixel = getSubPointsByDistance(listPointsOfLeft,20);


        // get list points from Rght
        vector<Point> listPointsOfRight = findMinXmappingY(rightFragment->getBorder());
        // get every 20 pixel
        vector<Point> listPointsOfRight20Pixel = getSubPointsByDistance(listPointsOfRight,20);

        // run the algorithm
        vector<int> vectorA = getXvalueOfListPoints(listPointsOfLeft20Pixel);
        vector<int> vectorB = getXvalueOfListPoints(listPointsOfRight20Pixel);
        vector< vector<int> > myDTW = cumulativeDistanceMatrix(vectorA,vectorB);
        vector< pair<int,int> > myPath = optimalWarpingPath(myDTW);
        int n = myPath.size();
        for(int i=0; i<n; i++)
        {
            int indexA = myPath[i].first;
            Point pointA = listPointsOfLeft20Pixel.at(indexA);
            int indexB = myPath[i].second;
            Point pointB = listPointsOfRight20Pixel.at(indexB);

            QRectF rectA = leftFragment->mapToScene(leftFragment->boundingRect()).boundingRect();\
            QRectF rectB = rightFragment->mapToScene(rightFragment->boundingRect()).boundingRect();

            QPen pen(Qt::DotLine);
            QGraphicsLineItem *line = scene->addLine(rectA.x()+pointA.getX(),rectA.y()+pointA.getY(),
                                                     rectB.x()+pointB.getX(),rectB.y()+pointB.getY(),pen);
            suggestLines.append(line);
            // add mapping point to draw
            listPointsBeginToDraw.push_back(pointA);
            listPointsEndToDraw.push_back(pointB);
        }
        leftFragment->setconnectedLines(suggestLines, true);
        leftFragment->setListConnectedPoints(listPointsBeginToDraw);

        rightFragment->setconnectedLines(suggestLines, false);
        rightFragment->setListConnectedPoints(listPointsEndToDraw);
    }
}

void WindowFragments::loadObject(string imageName, int locationX, int locationY, ptrRGBAMatrix objectRGBAMatrix, Edge border)
{
    QImage qImage = ptrRGBAToQImage(objectRGBAMatrix);

    FragmentItem *pixmapItem = new FragmentItem(imageName, locationX, locationY,
                                                border, objectRGBAMatrix, QPixmap::fromImage(qImage), fragmentMenu);

    scene->addItem(pixmapItem);
}

void WindowFragments::rotateCenter(QGraphicsItem *selectedItem, int angle)
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

void WindowFragments::createActions()
{
    createFileMenuActions();
    createFragmentMenuActions();
    createMatchingMenuActions();
    createHelpMenuActions();
}

void WindowFragments::createFileMenuActions()
{
    openAction = new QAction(QIcon("./resources/ico/open.png"), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void WindowFragments::createFragmentMenuActions()
{
    deleteAction = new QAction(QIcon("./resources/ico/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from screen"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    toFrontAction = new QAction(QIcon("./resources/ico/bringtofront.png"), tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon("./resources/ico/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    rotateLeftAction = new QAction(QIcon("./resources/ico/rotateleft.png"), tr("Rotate &Left"), this);
    rotateLeftAction->setShortcut(tr("Ctrl+L"));
    rotateLeftAction->setStatusTip(tr("Rotate left"));
    connect(rotateLeftAction, SIGNAL(triggered()), this, SLOT(rotateleft()));

    rotateRightAction = new QAction(QIcon("./resources/ico/rotateright.png"), tr("Rotate &Right"), this);
    rotateRightAction->setShortcut(tr("Ctrl+R"));
    rotateRightAction->setStatusTip(tr("Rotate Right"));
    connect(rotateRightAction, SIGNAL(triggered()), this, SLOT(rotateright()));

    setLeftFragmentAction = new QAction(QIcon("./resources/ico/rotateright.png"), tr("Set the left fragment"), this);
    setLeftFragmentAction->setStatusTip(tr("Set the left fragment"));
    connect(setLeftFragmentAction, SIGNAL(triggered()), this, SLOT(setLeftFragment()));

    setRightFragmentAction = new QAction(QIcon("./resources/ico/rotateright.png"), tr("Set the right fragment"), this);
    setRightFragmentAction->setStatusTip(tr("Set the right fragment"));
    connect(setRightFragmentAction, SIGNAL(triggered()), this, SLOT(setRightFragment()));

    propertyAction = new QAction(QIcon("./resources/ico/Information.png"), tr("&Property"), this);
    propertyAction->setStatusTip(tr("Information of fragment"));
    connect(propertyAction, SIGNAL(triggered()), this, SLOT(propertyItem()));
}

void WindowFragments::createMatchingMenuActions()
{
    suggestDTWAction = new QAction(QIcon("./resources/ico/matching.png"), tr("suggest matching by DTW"), this);
    suggestDTWAction->setStatusTip(tr("suggest matching by DTW"));
    connect(suggestDTWAction, SIGNAL(triggered()), this, SLOT(suggestDTW()));
}

void WindowFragments::createHelpMenuActions()
{
    aboutAction = new QAction(tr("&About MAELab"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void WindowFragments::createMenus()
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
    fragmentMenu->addSeparator();
    fragmentMenu->addAction(propertyAction);

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

void WindowFragments::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAction);

    fragmentToolBar = addToolBar(tr("Fragment"));
    fragmentToolBar->addAction(deleteAction);
    fragmentToolBar->addAction(toFrontAction);
    fragmentToolBar->addAction(sendBackAction);
    fragmentToolBar->addAction(rotateLeftAction);
    fragmentToolBar->addAction(rotateRightAction);

    matchingToolBar = addToolBar(tr("Matching"));
    matchingToolBar->addAction(suggestDTWAction);
}

QGraphicsItem *WindowFragments::selectedItem() const
{
    QList<QGraphicsItem *> items = scene->selectedItems();
    if(items.count()==1)
        return items.first();
    else
        return 0;
}

