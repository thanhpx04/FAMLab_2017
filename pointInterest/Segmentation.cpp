/*
 * Segmentation.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"
#include "../segmentation/Suzuki.h"
#include "../histograms/ShapeHistogram.h"

#include "Treatments.h"
#include "Segmentation.h"

Segmentation::Segmentation()
{
	// TODO Auto-generated constructor stub

}

Segmentation::~Segmentation()
{
	// TODO Auto-generated destructor stub
}

ptr_IntMatrix Segmentation::threshold(int tValue, int maxValue)
{
	ptr_IntMatrix inputMatrix = Treatments::refImage.getGrayMatrix();
	return binaryThreshold(inputMatrix, tValue, maxValue);
}

vector<ptr_Edge> Segmentation::canny()
{
	return Treatments::refImage.cannyAlgorithm();
}
vector<ptr_Line> Segmentation::segment(int minDistance = 3)
{
	return Treatments::refImage.getApproximateLines(minDistance);
}

