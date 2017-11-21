/********************************************************************************
** Form generated from reading UI file 'MatchingFragmentWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATCHINGFRAGMENTWINDOW_H
#define UI_MATCHINGFRAGMENTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatchingFragmentWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QPushButton *btnRotateLeft;
    QPushButton *btnRotateRight;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MatchingFragmentWindow)
    {
        if (MatchingFragmentWindow->objectName().isEmpty())
            MatchingFragmentWindow->setObjectName(QString::fromUtf8("MatchingFragmentWindow"));
        MatchingFragmentWindow->resize(1600, 1200);
        actionOpen = new QAction(MatchingFragmentWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(MatchingFragmentWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(MatchingFragmentWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 1440, 1021));
        btnRotateLeft = new QPushButton(centralwidget);
        btnRotateLeft->setObjectName(QString::fromUtf8("btnRotateLeft"));
        btnRotateLeft->setGeometry(QRect(1490, 0, 99, 27));
        btnRotateRight = new QPushButton(centralwidget);
        btnRotateRight->setObjectName(QString::fromUtf8("btnRotateRight"));
        btnRotateRight->setGeometry(QRect(1490, 40, 99, 27));
        MatchingFragmentWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MatchingFragmentWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1600, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MatchingFragmentWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MatchingFragmentWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MatchingFragmentWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MatchingFragmentWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MatchingFragmentWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        toolBar->addAction(actionOpen);

        retranslateUi(MatchingFragmentWindow);

        QMetaObject::connectSlotsByName(MatchingFragmentWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MatchingFragmentWindow)
    {
        MatchingFragmentWindow->setWindowTitle(QApplication::translate("MatchingFragmentWindow", "Papyrus FAMLab", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MatchingFragmentWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MatchingFragmentWindow", "Exit", 0, QApplication::UnicodeUTF8));
        btnRotateLeft->setText(QApplication::translate("MatchingFragmentWindow", "-1", 0, QApplication::UnicodeUTF8));
        btnRotateRight->setText(QApplication::translate("MatchingFragmentWindow", "+1", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MatchingFragmentWindow", "File", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MatchingFragmentWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MatchingFragmentWindow: public Ui_MatchingFragmentWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATCHINGFRAGMENTWINDOW_H
