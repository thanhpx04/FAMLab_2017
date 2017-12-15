/*
 * Canny.h
 *
 *  Created on: Oct 25, 2016
 *      Author: linh
 */

#ifndef CANNY_H_
#define CANNY_H_

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../io/Reader.h"
#include "../utils/Converter.h"
#include "Filters.h"
#include "Thresholds.h"

double getBorderValue(ptr_IntMatrix inputMatrix, int x, int y);
ptr_IntMatrix cannyProcess(ptr_IntMatrix binaryImage, int low, int high, vector<Point> &contourPoints);
ptr_IntMatrix cannyProcess2(ptr_IntMatrix binaryImage, int lowThreshold,
	int highThreshold, ptr_IntMatrix &gradDirection,vector<Point> &edgePoints);
ptr_IntMatrix postProcess(ptr_IntMatrix binaryMatrix, int maxValue);
Point findMinPointOfBorder(Edge border);
#endif /* CANNY2_H_ */
