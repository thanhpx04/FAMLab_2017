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

Segmentation::Segmentation() {
	// TODO Auto-generated constructor stub

}

Segmentation::~Segmentation() {
	// TODO Auto-generated destructor stub
}

ptr_IntMatrix Segmentation::threshold(int tValue, int maxValue) {
	ptr_IntMatrix inputMatrix = Treatments::refImage.getGrayMatrix();
	ptr_IntMatrix binaryMatrix = new Matrix<int>(inputMatrix->getRows(),
			inputMatrix->getCols(), maxValue);
	*binaryMatrix = *(binaryThreshold(inputMatrix, tValue, maxValue));
	ptr_IntMatrix result = new Matrix<int>(inputMatrix->getRows(),
			inputMatrix->getCols(), maxValue);
	*result = *(postProcess(binaryMatrix, maxValue));
	delete binaryMatrix;
	return result;
	//return binaryThreshold(inputMatrix, tValue, maxValue);
}

vector<Edge> Segmentation::canny() {
	return Treatments::refImage.cannyAlgorithm();
}
vector<Line> Segmentation::segment() {
	return Treatments::refImage.getListOfLines();
}
int Segmentation::removePronotum() {
	Image image = Treatments::refImage;
	ptr_IntMatrix histogram = image.getHistogram();
	ptr_IntMatrix grayScale = image.getGrayMatrix();
	int histCols = histogram->getCols();
	int max = -1, imax = -1;
	for (int i = 0; i < histCols / 2; i++) {
		if (histogram->getAtPosition(0, i) > max) {
			imax = i;
			max = histogram->getAtPosition(0, i);
		}
	}
	int min = max, imin = -1;
	cout << "\n Max location: " << imax << endl;
	for (int j = imax; j < 2 * histCols / 3; j++) {
		if (histogram->getAtPosition(0, j) <= min) {
			imin = j;
			min = histogram->getAtPosition(0, j);
		} else {
			break;
		}
	}
	cout << "\n Min location: " << imin << endl;
	max = min;
	int imax2 = imin;
	for (int k = imin; k < histCols; k++) {
		if (histogram->getAtPosition(0, k) >= min) {
			imax2 = k;
			max = histogram->getAtPosition(0, k);
		} else {
			break;
		}
	}
	cout << "\n Max2 location: " << imax2 << endl;
	return imin;
}
