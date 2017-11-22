TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += "/usr/local/include"
LIBS += -lpng -L"/usr/lib"
	

QT           += opengl
CONFIG       += console
#RESOURCES     = Ipm.qrc
QMAKE_CXXFLAGS += -fPIC -lpthread
QMAKE_CFLAGS += -fPIC -lpthread
HEADERS += io/*.h io/LibJpeg/*.h imageModel/*.h \
			segmentation/*.h histograms/*.h pht/*.h \
                  pointInterest/*.h algorithm/*.h correlation/*.h utils/*.h ui/*.h MAELab.h

	
SOURCES += io/*.cpp io/LibJpeg/*.c imageModel/*.cpp \
		 segmentation/*.cpp \
                 histograms/*.cpp pht/*.cpp pointInterest/*.cpp algorithm/*.cpp correlation/*.cpp \
                 utils/*.cpp ui/*.cpp MAELab.cpp FAMLab.cpp \



