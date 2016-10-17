/*
 * Image.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: linh
 */

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "Point.h"
#include "Line.h"
#include "Edge.h"
#include "Matrix.h"
#include "../io/Reader.h"

#include "Image.h"
#include "../utils/Canny.h"

const int BIN_SIZE = 256;
const int MEAN_GRAY = 120;
const int DECREASE_25 = 25;
const int DECREASE_5 = 5;

//================================================= Utils methods =================================================
ptr_IntMatrix convertRGBToGray(ptr_RGBMatrix rgbMatrix) {
	ptr_IntMatrix grayMatrix;
	unsigned int width = rgbMatrix->getCols();
	unsigned int height = rgbMatrix->getRows();

	grayMatrix = new Matrix<unsigned int>(height, width);
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {

			grayMatrix->setAtPosition(h, w,
					((rgbMatrix->getAtPosition(h, w).R
							+ rgbMatrix->getAtPosition(h, w).G
							+ rgbMatrix->getAtPosition(h, w).B) / 3));
		}
	}

	return grayMatrix;
}
// ================================================== End utils methods =============================================
//===================================================== Constructor =================================================
Image::Image() {
	// TODO Auto-generated constructor stub

}

Image::~Image() {
	// TODO Auto-generated destructor stub
}
Image::Image(std::string filePath) {
	fileName = filePath;
	imgMatrix = readJPGToRGB(filePath.c_str());
	grayMatrix = convertRGBToGray(imgMatrix);
	calcGrayHistogram();
	calThresholdValue();
}

//===================================================== End constructor ================================================

//================================================ Public methods ======================================================
void Image::setFileName(std::string filePath) {
	fileName = filePath;
}
std::string Image::getFileName() {
	return fileName;
}
/*void Image::setEdges(std::vector<Edge> edges){
 listOfEdges = edges;
 }
 std::vector<Edge> Image::getEdge(){
 return listOfEdges;
 }*/
void Image::setMLandmarks(string tpsFile) {
	listOfMLandmarks = readTPSFile(tpsFile.c_str());

}
ptr_IntMatrix Image::getGrayMatrix() {
	return grayMatrix;
}
ptr_RGBMatrix Image::getRGBMatrix() {
	return imgMatrix;
}
float Image::getMedianHistogram() {
	if (medianHistogram == 0)
		calcGrayHistogram();
	return medianHistogram;

}
float Image::getMeanHistogram() {
	if (meanHistogram == 0)
		calcGrayHistogram();
	return meanHistogram;
}
float Image::getThresholdValue() {
	if (thresholdValue == 0)
		calThresholdValue();
	return thresholdValue;
}

vector<ptr_Line> Image::getApproximateLines(int minDistance){
	vector<ptr_Line> lines;
	for (size_t t = 0; t < listOfEdges.size(); ++t) {
		ptr_Edge edgei = listOfEdges.at(t);
		vector<ptr_Line> templines = edgei->segment(minDistance);
		lines.insert(lines.end(),templines.begin(),templines.end());

	}
	return lines;
}
//================================================ End public methods ==================================================

//================================================ Private methods =====================================================
void Image::calcGrayHistogram() {

	if (grayMatrix->getCols() != 0) {

		float total = 0;
		float pi = 0;
		int array[BIN_SIZE] = { 0 };

		for (int c = 0; c < grayMatrix->getRows(); c++) {
			for (int r = 0; r < grayMatrix->getCols(); r++) {
				int k = grayMatrix->getAtPosition(c, r);
				array[k]++;
			}
		}

		grayHistogram = new Matrix<unsigned int>(1, BIN_SIZE, 0);

		for (int k = 0; k < BIN_SIZE; k++) {
			grayHistogram->setAtPosition(0, k, 255);
			total += array[k];
			pi += (k * array[k]);
		}

		// calculate the mean of histogram
		meanHistogram = (pi / total);

		// calculate the median of histogram
		float avm = total / 2;
		float temp = 0;
		for (int m = 0; m < BIN_SIZE; m++) {
			temp += array[m];
			if (temp >= avm) {
				medianHistogram = m;
				break;
			}
		}
	}

}

void Image::calThresholdValue() {
	if (medianHistogram == 0 || meanHistogram == 0)
		calcGrayHistogram();
	int limit1 =
			meanHistogram > medianHistogram ? medianHistogram : meanHistogram;
	limit1 = (limit1 >= 120) ? (limit1 - DECREASE_25) : (limit1 - DECREASE_5);
	int imax1 = -1, max1 = -1;
	for (int index = 0; index < limit1; index++) {
		int temp = grayHistogram->getAtPosition(0, index);
		if (temp > max1) {
			max1 = temp;
			imax1 = index;
		}
	}
	int limit2 =
			meanHistogram > medianHistogram ? meanHistogram : medianHistogram;
	int imin = -1, min = max1;
	for (int k = imax1; k < limit2; k++) {
		int temp = grayHistogram->getAtPosition(0, k);
		if (temp < min) {
			min = temp;
			imin = k;
		}
	}
	int max2 = -1, imax2 = -1;
	for (int k = limit2; k < BIN_SIZE; k++) {
		int temp = grayHistogram->getAtPosition(0, k);
		if (temp > max2) {
			max2 = temp;
			imax2 = k;
		}
	}
	float mid1 = (imin + imax1) / 2;
	float mid2 = (imin + imax2) / 2;
	thresholdValue = (mid1 + mid2) / 2;
}

vector<ptr_Edge> Image::cannyAlgorithm() {
	if (thresholdValue == 0)
		calThresholdValue();
	vector<vector<ptr_Point> > edges = canny(grayMatrix, thresholdValue,
			3 * thresholdValue);
	for (size_t t = 0; t < edges.size(); ++t) {
		ptr_Edge edge = new Edge(edges.at(t));
		listOfEdges.push_back(edge);
	}
	return listOfEdges;

}
//================================================ End private methods =====================================================

