
#include <iostream>

using namespace std;

#include <QtGui/QApplication>
#include "ui/MatchingFragmentWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MatchingFragmentWindow window;
    window.setGeometry(100, 100, 800, 500);
    window.show();

	return app.exec();
	return 0;

}
