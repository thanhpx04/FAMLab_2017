	
#COMPILER: GCC FOR C PROGRAM, G++ FOR c++
CC = g++

#COMPILER FLAG
CFLAGS 		= -g -Wall -fopenmp -O2 -pthread
CPPFLAGS 	= -I/usr/include/freetypes -I/usr/include/libxml2
CXX			= g++
CXXFLAGS 	= -pthread
DEFS		= -DHAVE_CONFIG_h
LDFLAGS		= -L/usr/lib/x86_64-linux-gnu
LIBS		= -ljbig -lwebp -llcms2 -ltiff -lfreetype -ljasper -ljpeg -lpng12 -lwmflite -lXext -lSM -lICE -lX11 -llzma -lbz2 -lxml2 -lz -lm -lgomp -lpthread

IMGModel 	= imageModel
IOModel 	= io
HELPModel	= utils
HISTModel	= histograms
#SOURCES		= main.cpp $(IMGModel)/.cpp $(IOModel)/.cpp
#OBJECTS		= $(SOURCES:.cpp=.o)
#default:main

main: main.o $(HELPModel)/Canny.o  $(IMGModel)/Point.o $(IMGModel)/Line.o $(IMGModel)/Edge.o $(IMGModel)/Matrix.o $(IMGModel)/Image.o $(IOModel)/TPSReader.o $(IOModel)/SaveJPG.o $(IOModel)/LoadJPG.o $(IOModel)/Reader.o $(HISTModel)/ShapeHistogram.o
			 
	$(CC) $(CFLAGS) -std=c++11 -o main main.o $(HELPModel)/Canny.o $(IMGModel)/Point.o $(IMGModel)/Line.o $(IMGModel)/Edge.o $(IMGModel)/Matrix.o $(IMGModel)/Image.o $(IOModel)/TPSReader.o $(IOModel)/SaveJPG.o $(IOModel)/LoadJPG.o $(IOModel)/Reader.o $(HISTModel)/ShapeHistogram.o

main.o: main.cpp $(IMGModel)/Point.h $(IMGModel)/Line.h $(IMGModel)/Edge.h $(IMGModel)/Matrix.h $(IMGModel)/Image.h $(IOModel)/TPSReader.h $(IOModel)/SaveJPG.h $(IOModel)/LoadJPG.h $(IOModel)/Reader.h $(HELPModel)/Canny.h $(HISTModel)/ShapeHistogram.h
	$(CC) $(CFLAGS) -c main.cpp $(IMGModel)/Point.cpp $(IMGModel)/Line.cpp $(IMGModel)/Edge.cpp $(IMGModel)/Matrix.cpp $(IMGModel)/Image.cpp $(IOModel)/TPSReader.cpp $(IOModel)/SaveJPG.cpp $(IOModel)/LoadJPG.cpp $(IOModel)/Reader.cpp $(HELPModel)/Canny.cpp $(HISTModel)/ShapeHistogram.cpp

#Point.o: $(IMGModel)/Point.h
#Line.o: $(IMGModel)/Line.h
#Edge.o: $(IMGModel)/Edge.h
#Matrix.o: $(IMGModel)/Matrix.h
#Image.o: $(IMGModel)/Image.h
#TPSReader.o: $(IOModel)/TPSReader.h
#SaveJPG.o: $(IOModel)/SaveJPG.h
#LoadJPG.o: $(IOModel)/LoadJPG.h
#Reader.o: $(IOModel)/Reader.h
#Canny.o: $(HELPModel)/Canny.h





clean:
	rm -f main
	rm -f *.o
	rm -f imageModel/*.o
	rm -f io/*.o
	rm -f utils/*.o
	rm -f histograms/*.o