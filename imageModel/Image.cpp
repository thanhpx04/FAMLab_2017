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
#include "../utils/Canny.h"
#include "../utils/Suzuki.h"

#include "Image.h"

const int BIN_SIZE = 256;
const int MEAN_GRAY = 120;
const int DECREASE_25 = 25;
const int DECREASE_5 = 5;

const double RED_COEFFICIENT = 0.299f;
const double GREEN_COEFFICIENT = 0.587f;
const double BLUE_COEFFICIENT = 0.114f;

const int MAX_GRAY_VALUE = 255;
/*
 * The equations to convert from RGB or BGR to Grayscale: Gvalue = 0.299*R + 0.587*G + 0.114*B
 *
 */
//================================================= Utils methods =================================================
ptr_IntMatrix convertRGBToGray(ptr_RGBMatrix rgbMatrix) {
	ptr_IntMatrix grayMatrix;
	unsigned int width = rgbMatrix->getCols();
	unsigned int height = rgbMatrix->getRows();

	grayMatrix = new Matrix<unsigned int>(height, width);
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {

			grayMatrix->setAtPosition(h, w,
					round(
							((double) rgbMatrix->getAtPosition(h, w).R
									* RED_COEFFICIENT)
									+ ((double) rgbMatrix->getAtPosition(h, w).G
											* GREEN_COEFFICIENT)
									+ ((double) rgbMatrix->getAtPosition(h, w).B
											* BLUE_COEFFICIENT)));
		}
	}

	return grayMatrix;
}

ptr_IntMatrix binaryThreshold(ptr_IntMatrix inputMatrix, int tValue,
		int maxValue) {
	int rows = inputMatrix->getRows();
	int cols = inputMatrix->getCols();

	ptr_IntMatrix result = new Matrix<unsigned int>(rows, cols);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (inputMatrix->getAtPosition(r, c) > tValue)
				result->setAtPosition(r, c, maxValue);
			else
				result->setAtPosition(r, c, 0);
		}
	}
	return result;
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
	/*ofstream of("output/image Value.txt");
	 for (int r = 0; r < imgMatrix->getRows(); ++r) {
	 for (int c = 0; c < imgMatrix->getCols(); ++c) {
	 of << (int) imgMatrix->getAtPosition(r, c).B << "\t"
	 << (int) imgMatrix->getAtPosition(r, c).G << "\t"
	 << (int) imgMatrix->getAtPosition(r, c).R<<"\t"
	 << (int) grayMatrix->getAtPosition(r,c)<<"\n";

	 }
	 }
	 of.close();*/

	cout << endl << "Threshold value: " << thresholdValue;
	/*cout << endl << "Test value in histogram matrix: "
	 << (int) grayHistogram->getAtPosition(0, 200);
	 cout << endl << "The histogram values: " << endl;
	 for (int i = 0; i < 256; i++) {
	 cout << "\t" << grayHistogram->getAtPosition(0, i);
	 }*/
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
	//listOfMLandmarks = readTPSFile(tpsFile.c_str());

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

vector<ptr_Line> Image::getApproximateLines(int minDistance = 3) { // min distance to seperate the edge to list of approximate lines
	vector<ptr_Edge> listOfEdges = cannyAlgorithm();
	cout << "\n Number of edges: " << listOfEdges.size();
	vector<ptr_Line> lines;
	for (size_t t = 0; t < listOfEdges.size(); t++) {
		ptr_Edge edgei = listOfEdges.at(t);
		vector<ptr_Line> templines = edgei->segment(minDistance);

		if (templines.size() > 0)
			lines.insert(lines.end(), templines.begin(), templines.end());

	}
	cout << "\n Finish segment to lines, total lines in images are "
			<< lines.size();
	listOfLines = lines;
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
		/*cout<<endl<<"value of histogram:"<<endl;
		 for (int i = 0; i < 256; i++) {
		 cout << "\t" << array[i];
		 }*/

		grayHistogram = new Matrix<unsigned int>(1, BIN_SIZE, 0);

		for (int k = 0; k < BIN_SIZE; k++) {
			grayHistogram->setAtPosition(0, k, array[k]);
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

	ptr_IntMatrix binMatrix = binaryThreshold(grayMatrix, thresholdValue,
			MAX_GRAY_VALUE);

	vector<ptr_Edge> listOfEdges = cannyProcess(binMatrix, (int) thresholdValue,
			3 * (int) thresholdValue);

	//suzuki(binMatrix);

	/*int count = 0;
	 for (int i = 0; i < listOfEdges.size(); i++) {
	 if (listOfEdges.at(i)->getPoints().size() > 3)
	 count++;
	 }

	 cout << "\n Number of edges: " << listOfEdges.size();
	 cout << "\n Number of edges > 30: " << count;*/
	return listOfEdges;

}
//================================================ End private methods =====================================================

