TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += "/usr/local/include"
LIBS += -L"/usr/lib" -lOpenCL \
	

QT           += opengl
CONFIG       += console
#RESOURCES     = Ipm.qrc
QMAKE_CXXFLAGS += -fPIC -lpthread
QMAKE_CFLAGS += -fPIC -lpthread
HEADERS += io/*.h io/LibJpeg/*.h imageModel/Point.h imageModel/Line.h imageModel/Edge.h imageModel/Matrix.h imageModel/Image.h \
			segmentation/*.h histograms/*.h pht/*.h \
		  pointInterest/*.h correlation/*.h utils/*.h ui/*.h MAELab.h 
	
SOURCES += MAELab_CI.cpp io/*.cpp io/LibJpeg/*.c imageModel/Point.cpp imageModel/Line.cpp imageModel/Edge.cpp imageModel/Matrix.cpp imageModel/Image.cpp \
		segmentation/*.cpp \
		 histograms/*.cpp pht/*.cpp pointInterest/*.cpp correlation/*.cpp \
		 utils/*.cpp ui/*.cpp MAELab.cpp 