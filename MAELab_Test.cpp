/*
 *
 *
 * Test file
 */
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <pthread.h>

using namespace std;

#include "imageModel/Point.h"
#include "imageModel/Line.h"
#include "imageModel/Edge.h"
#include "imageModel/Matrix.h"
#include "imageModel/Image.h"
#include "utils/Drawing.h"
#include "io/Reader.h"

#include "segmentation/Thresholds.h"
#include "segmentation/Canny.h"
#include "segmentation/Filters.h"
#include "segmentation/Projection.h"
#include "correlation/DescriptorDistance.h"

/**
 * This function is used to fill the hole inside the object (pronotum)
 * Input: file path of rgb image
 * Output: the output image under Binary format.
 */

ptr_IntMatrix holeFill(string filename, string savename)
{
	Image image(filename);
	Matrix<double> gauKernel = getGaussianKernel(5, 1);
	Matrix<int> result = gaussianBlur(*image.getGrayMatrix(), gauKernel);
	ptr_IntMatrix binMatrix = binaryThreshold(&result, image.getThresholdValue(),
		255);
	binMatrix = postProcess(binMatrix, 255);
	saveGrayScale(savename.c_str(), binMatrix);
	return binMatrix;
}

Matrix<int> removelegMain(string filename, string savename)
{
	ptr_IntMatrix binMatrix = holeFill(filename, savename);
	Matrix<int> result = splitImage(binMatrix, 150); // 150 --> height of sub-image
	saveGrayScale(savename.c_str(), &result);
	return result;
}

void getProjections(string filename, string savename)
{
	Matrix<int> binMatrix = removelegMain(filename, savename);
	ptr_IntMatrix hProjection = new Matrix<int>(binMatrix.getRows(),
		binMatrix.getCols(), 255);
	ptr_IntMatrix vProjection(hProjection);
	binProjection(&binMatrix, hProjection, vProjection);
	analysisHistogram(hProjection, 0, 20);
	saveGrayScale(savename.c_str(), hProjection);
}

void colorThreshold(string filename, string savename)
{
	Image matImage(filename);
	ptr_RGBMatrix histogram = matImage.getRGBHistogram();
	double totalPixels = matImage.getGrayMatrix()->getRows()
		* matImage.getGrayMatrix()->getCols();
	ptr_RGBMatrix result = colorThreshold(matImage.getRGBMatrix(), histogram);
	saveRGB(savename.c_str(), result);
}

void extractLandmarkPatch(string image_file, string landmark_file, int width,
	int height, string save_folder)
{
	Image matImage(image_file);
	matImage.readManualLandmarks(landmark_file);
	vector<Point> landmarks = matImage.getListOfManualLandmarks();
	string name = matImage.getName();
	size_t found2 = name.find_last_of(".");
	string sname = name.substr(0, found2);

	RGB color;
	color.R = color.G = color.B = 0;
	for (int i = 0; i < landmarks.size(); i++)
	{
		Point pi = landmarks.at(i);
		Matrix<int> patch = matImage.getGrayMatrix()->extractPatch(width, height,
			pi.getY(), pi.getX(), 0);
		std::stringstream ssname;
		ssname << sname;
		ssname << "_p" << i << ".jpg";
		string savename = save_folder + "/" + ssname.str();
		saveGrayScale(savename.c_str(), &patch);
	}
}

void calculateSIFTDescriptor(string image_file, Point point, int patchsize)
{
	Image matImage(image_file);
	vector<double> SIFTVector = SIFTDescriptor(matImage.getGrayMatrix(), point, patchsize);
	for (size_t i = 0; i < SIFTVector.size(); ++i) {
		cout<<endl<<SIFTVector.at(i);
	}
}

int main(int argc, char* argv[])
{
	cout << "\n Test a function !!!" << endl;

	// ================================================================ Test hole fill =================================================
	cout << endl << "\n Hole fill" << endl;
	string filename, savename, lm_file;
	int width, height;
	string save_folder;
	if (argc == 1)
	{
		cout << "\nWithout parameters !!" << endl;
		filename = "data/md028.jpg";
		savename = "results/md028.jpg";
		lm_file = "data/landmarks/Md 028.TPS";
		width = 121;
		height = 121;
		save_folder = "results";
	}
	else
	{
		cout << "\nWith parameters !!" << endl;
		filename = argv[1];
		//savename = argv[2];
		lm_file = argv[2];
		width = atoi(argv[3]);
		height = atoi(argv[4]);
		save_folder = argv[5];
	}
	//holeFill(filename,savename);
	//removelegMain(filename, savename);
	//getProjections(filename,savename);
	//colorThreshold(filename, savename);
	//extractLandmarkPatch(filename, lm_file, width, height, save_folder);
	calculateSIFTDescriptor(filename,Point(1592,1175),9);

}
