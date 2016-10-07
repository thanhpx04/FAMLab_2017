/*
 * Image.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

using namespace std;

#include "Point.h"
#include "Line.h"
#include "Edge.h"
#include "Matrix.h"
#include "../io/Reader.h"

#include "Image.h"

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

}
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

void Image::calcGrayHistogram() {


	if (grayMatrix->getCols() != 0) {
		grayHistogram = new Matrix<unsigned int>(grayMatrix->getCols() * grayMatrix->getRows(),256,0);
		float total = 0;
		float pi = 0;
		int array[256] = {0};

		for (int c = 0; c < grayMatrix->getRows(); c++) {
			for (int r = 0; r < grayMatrix->getCols(); r++) {
				int k = grayMatrix->getAtPosition(c, r);
				array[k]++;

			}
		}

		for (int k = 0; k < 256; k++) {
			grayHistogram->setAtPosition(array[k],k,1);
			total += array[k];
			pi += (k * array[k]);
		}

		// calculate the mean of histogram
		meanHistogram = (pi / total);

		// calculate the median of histogram
		float avm = total / 2;
		float temp = 0;
		for (int m = 0; m < 256; m++) {
			temp += array[m];
			if (temp >= avm) {
				medianHistogram = m;
				break;
			}
		}
	}

}

