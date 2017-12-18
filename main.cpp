#include "ui/WindowFragments.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    ImageViewer imageViewer;
//#if defined(Q_OS_SYMBIAN)
//    imageViewer.showMaximized();
//#else
//    imageViewer.show();
//#endif

    WindowFragments window;
    window.setGeometry(100, 100, 800, 500);
    window.show();

    return a.exec();
}
