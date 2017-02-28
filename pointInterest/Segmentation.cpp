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

#include "../io/Reader.h"

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

vector<Edge> Segmentation::canny()
{
	return Treatments::refImage.cannyAlgorithm();
}
vector<Line> Segmentation::segment()
{
	return Treatments::refImage.getListOfLines();
}
int Segmentation::removePronotum()
{
	Image image = Treatments::refImage;
	ptr_IntMatrix histogram = image.getHistogram();
	ptr_IntMatrix grayScale = image.getGrayMatrix();
	int histCols = histogram->getCols();
	int max = -1, imax = -1;
	for (int i = 0; i < histCols / 2; i++)
	{
		if (histogram->getAtPosition(0, i) > max)
		{
			imax = i;
			max = histogram->getAtPosition(0, i);
		}
	}
	int min = max, imin = -1;
	cout << "\n Max location: " << imax << endl;
	for (int j = imax + 2; j < 2 * histCols / 3; j++)
	{
		int l2 = histogram->getAtPosition(0, j - 2);
		int l1 = histogram->getAtPosition(0, j - 1);
		int v = histogram->getAtPosition(0, j);
		int r1 = histogram->getAtPosition(0, j + 1);
		int r2 = histogram->getAtPosition(0, j + 2);
		if (histogram->getAtPosition(0, j) <= min && v <= l1 && v <= l2 && v <= r1
			&& v <= r2)
		{
			imin = j;
			min = histogram->getAtPosition(0, j);
			break;
		}
	}
	cout << "\n Min location: " << imin << endl;
	return imin-5;
}
void Segmentation::gridRemoveFolder()
{
	string path = "/home/linh/Datasets/Morphometrics/pronotum/Original_images";
	vector<string> images = readDirectory(path.c_str());
	string saveFolder = "/home/linh/Desktop/test";
	for (size_t i = 80; i < 100; i++) {
		string sceneName = images.at(i);
		Image image(path +"/"+sceneName);
		setRefImage(image);
		int t = removePronotum();
		ptr_IntMatrix vl = threshold(t,255);
		saveGrayScale((saveFolder+"/"+sceneName).c_str(),vl);
	}
}
