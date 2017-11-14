
#include <iostream>

using namespace std;

#include <QtGui/QApplication>
#include "ui/MatchingFragmentWindow.h"

int main(int argc, char* argv[])
{
    cout << "\n Papyrus FAMLab with graphic user interface !!!" << endl;
    QApplication app(argc, argv);

    MatchingFragmentWindow window;
#if defined(Q_OS_SYMBIAN)
    window.showMaximized();
#else
    window.show();
#endif
	return app.exec();
	return 0;

}
