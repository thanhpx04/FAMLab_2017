#ifndef REGIONGROWING_H
#define REGIONGROWING_H
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <float.h>

using namespace std;
#include "../MAELab/imageModel/Point.h"
#include "../MAELab/imageModel/Line.h"
#include "../MAELab/imageModel/Edge.h"
#include "../MAELab/imageModel/Matrix.h"

void detectRegion(ptr_IntMatrix mask, float thresholdValue, Point startingPoint, int &minX, int &maxX, int &minY, int &maxY);
vector<Point> growRegion(ptr_IntMatrix mask, float thresholdValue, Point checkingPoint, int &minX, int &maxX, int &minY, int &maxY);



#endif // REGIONGROWING_H
