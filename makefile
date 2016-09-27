	
#COMPILER: GCC FOR C PROGRAM, G++ FOR c++
CC = g++

#COMPILER FLAG
CFLAGS = -g -Wall

IMGModel = imageModel

#default:main

main: main.o $(IMGModel)/Point.o $(IMGModel)/Line.o $(IMGModel)/Edge.o $(IMGModel)/Matrix.o $(IMGModel)/Image.o
	$(CC) $(CFLAGS) -o main main.o $(IMGModel)/Point.o $(IMGModel)/Line.o $(IMGModel)/Edge.o $(IMGModel)/Matrix.o $(IMGModel)/Image.o

main.o: main.cpp $(IMGModel)/Point.h $(IMGModel)/Line.h $(IMGModel)/Edge.h $(IMGModel)/Matrix.h $(IMGModel)/Image.h
	$(CC) $(CFLAGS) -c main.cpp $(IMGModel)/Point.cpp $(IMGModel)/Line.cpp $(IMGModel)/Edge.cpp $(IMGModel)/Matrix.cpp $(IMGModel)/Image.cpp

Point.o: $(IMGModel)/Point.h
Line.o: $(IMGModel)/Line.h
Edge.o: $(IMGModel)/Edge.h
Matrix.o: $(IMGModel)/Matrix.h
Image.o: $(IMGModel)/Image.h

clean:
	rm -f main
	rm -f *.o
	rm -f imageModel/*.o