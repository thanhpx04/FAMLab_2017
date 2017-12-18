#include "ui/WindowFragments.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WindowFragments window;
    window.setGeometry(100, 100, 800, 500);
    window.show();

    return a.exec();
}
