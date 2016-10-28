/*
 * Canny.cpp
 *
 *  Created on: Oct 25, 2016
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
#include "../imageModel/Matrix.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../io/Reader.h"

ptr_DoubleMatrix angles;
const float LIMIT_ANGLE_1 = 22.5;
const float LIMIT_ANGLE_2 = 67.5;
const float LIMIT_ANGLE_3 = 112.5;
const float LIMIT_ANGLE_4 = 157.5;

const float ANGLE_0 = 0.0;
const float ANGLE_45 = 45.0;
const float ANGLE_90 = 90.0;
const float ANGLE_135 = 135.0;
const float ANGLE_180 = 180.0;

#include "Canny.h"

vector<vector<double> > createGaussianFilter(int ksize, double sigmaIn) {

	vector<vector<double> > gFilter;

	if(ksize > 3){
		cout<<"Fail to create the gaussian filter. The size of filter too large. It should be 1(3x3), 2(5x5), or 3(7x7)";
		return gFilter;
	}
	int row = 2 * ksize + 1 ;
	int column = 2* ksize + 1;

	for (int i = 0; i < row; i++) {
		vector<double> col;
		for (int j = 0; j < column; j++) {
			col.push_back(-1);
		}
		gFilter.push_back(col);
	}

	float coorSum = 0;
	float constant = 2.0 * sigmaIn * sigmaIn;

	float sum = 0.0;

	for (int x = 1; x <= row; x++) {
		for (int y = 1; y <= column; y++) {
			float xSum = (x - (ksize + 1)) * (x - (ksize + 1));
			float ySum = (y - (ksize + 1)) * (y - (ksize + 1));
			coorSum = xSum + ySum;
			gFilter[x-1][y-1] = (exp(-(coorSum) / constant))
					/ (M_PI * constant);
			sum += gFilter[x - 1][y-1];
		}
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			gFilter[i][j] /= sum;
		}
	}
	return gFilter;
}

ptr_IntMatrix applyGaussianFilter(ptr_IntMatrix binaryImage,
		vector<vector<double> > gaussianFilter) {

	int size = (int) gaussianFilter.size();
	int ksize = size/2;

	ptr_IntMatrix filteredImg = new Matrix<unsigned int>();
	filteredImg = binaryImage;

	for (int i = 0; i < binaryImage->getRows() - size; i++) {
		for (int j = 0; j < binaryImage->getCols() - size; j++) {
			double sum = 0;
			for (int x = 0; x < size; x++) {
				for (int y = 0; y < size; y++) {
					sum += gaussianFilter[x][y]
							* (double) (binaryImage->getAtPosition(i + x,
									j + y));
				}
			}
			filteredImg->setAtPosition(i + ksize, j +ksize, sum);
		}
	}

	return filteredImg;

}

ptr_IntMatrix sobelOperation(ptr_IntMatrix gaussianImage) {

	double x1[] = { -1.0, 0, 1.0 };
	double x2[] = { -2.0, 0, 2.0 };
	double x3[] = { -1.0, 0, 1.0 };
	vector<vector<double> > xfilter(3);
	xfilter[0].assign(x1, x1 + 3);
	xfilter[1].assign(x2, x2 + 3);
	xfilter[2].assign(x3, x3 + 3);

	double y1[] = { -1.0, -2.0, -1.0 };
	double y2[] = { 0, 0, 0 };
	double y3[] = { 1.0, 2.0, 1.0 };
	vector<vector<double> > yfilter(3);
	yfilter[0].assign(y1, y1 + 3);
	yfilter[1].assign(y2, y2 + 3);
	yfilter[2].assign(y3, y3 + 3);

	int size = (int) xfilter.size();
	int ksize = size/2;

	ptr_IntMatrix filteredImg = new Matrix<unsigned int>(
			gaussianImage->getRows(), gaussianImage->getCols(),0);

	angles = new Matrix<double>(gaussianImage->getRows(),
			gaussianImage->getCols(),90);
	for (int i = 0; i < gaussianImage->getRows() - size; i++) {
		for (int j = 0; j < gaussianImage->getCols() - size; j++) {
			double sumx = 0, sumy = 0;
			for (int x = 0; x < size; x++) {
				for (int y = 0; y < size; y++) {
					sumx += xfilter[x][y]
							* (double) (gaussianImage->getAtPosition(i + x,
									j + y));
					sumy += yfilter[x][y]
							* (double) (gaussianImage->getAtPosition(i + x,
									j + y));
				}
			}
			double sumxsq = sumx * sumx;
			double sumysq = sumy * sumy;

			double sq2 = sqrt(sumxsq + sumysq);
			if (sq2 > 255)
				sq2 = 255;
			filteredImg->setAtPosition(i + ksize, j + ksize, sq2);
			if (sumx == 0)
				angles->setAtPosition(i + ksize, j + ksize, 90);
			else
				angles->setAtPosition(i + ksize, j + ksize, atan(sumy / sumx));
		}
	}
	return filteredImg;

}

ptr_IntMatrix nonMaxSuppression(ptr_IntMatrix sobelImage) {

	ptr_IntMatrix nonMaxSupped = new Matrix<unsigned int>(
			sobelImage->getRows(), sobelImage->getCols(),0);

	for (int i = 1; i < sobelImage->getRows() - 1; i++) {
		for (int j = 1; j < sobelImage->getCols() - 1; j++) {

			double tangent = angles->getAtPosition(i, j);
			nonMaxSupped->setAtPosition(i, j,
					sobelImage->getAtPosition(i, j));

			//Horizontal Edge
			if (((-LIMIT_ANGLE_1 < tangent) && (tangent <= LIMIT_ANGLE_1))
					|| ((LIMIT_ANGLE_4 < tangent) && (tangent <= -LIMIT_ANGLE_4))) {
				if ((sobelImage->getAtPosition(i, j)
						< sobelImage->getAtPosition(i, j + 1))
						|| (sobelImage->getAtPosition(i, j)
								< sobelImage->getAtPosition(i, j - 1)))
					nonMaxSupped->setAtPosition(i, j, 0);

			}
			//Vertical Edge
			if (((-LIMIT_ANGLE_3 < tangent) && (tangent <= -LIMIT_ANGLE_2))
					|| ((LIMIT_ANGLE_2 < tangent) && (tangent <= LIMIT_ANGLE_3))) {
				if ((sobelImage->getAtPosition(i, j)
						< sobelImage->getAtPosition(i + 1, j))
						|| (sobelImage->getAtPosition(i, j)
								< sobelImage->getAtPosition(i - 1, j)))
					nonMaxSupped->setAtPosition(i, j, 0);

			}

			//-45 Degree Edge
			if (((-LIMIT_ANGLE_2 < tangent) && (tangent <= -LIMIT_ANGLE_1))
					|| ((LIMIT_ANGLE_3 < tangent) && (tangent <= LIMIT_ANGLE_4))) {
				if ((sobelImage->getAtPosition(i, j)
						< sobelImage->getAtPosition(i - 1, j + 1))
						|| (sobelImage->getAtPosition(i, j)
								< sobelImage->getAtPosition(i + 1, j - 1)))
					nonMaxSupped->setAtPosition(i, j, 0);

			}

			//45 Degree Edge
			if (((-LIMIT_ANGLE_4 < tangent) && (tangent <= -LIMIT_ANGLE_3))
					|| ((LIMIT_ANGLE_1 < tangent) && (tangent <= LIMIT_ANGLE_2))) {
				if ((sobelImage->getAtPosition(i, j)
						< sobelImage->getAtPosition(i + 1, j + 1))
						|| (sobelImage->getAtPosition(i, j)
								< sobelImage->getAtPosition(i - 1, j - 1)))
					nonMaxSupped->setAtPosition(i, j, 0);

			}

		}
	}
	return nonMaxSupped;
}
ptr_IntMatrix doubleThreshold(ptr_IntMatrix nonMaxImage, int low, int high) {
	if (low > 255)
		low = 255;
	if (high > 255)
		high = 255;

	ptr_IntMatrix edgeMatrix = new Matrix<unsigned int>(nonMaxImage->getRows(),
			nonMaxImage->getCols());
	for (int i = 0; i < nonMaxImage->getRows(); i++) {
		for (int j = 0; j < nonMaxImage->getCols(); j++) {
			edgeMatrix->setAtPosition(i, j, nonMaxImage->getAtPosition(i, j));
			if (edgeMatrix->getAtPosition(i, j) >= high)
				edgeMatrix->setAtPosition(i, j, 255);
			else {
				if (edgeMatrix->getAtPosition(i, j) < low)
					edgeMatrix->setAtPosition(i, j, 0);
				else {
					bool anyHigh = false;
					bool anyBetween = false;
					for (int x = i - 1; x < i + 2; x++) {
						for (int y = j - 1; y < j + 2; y++) {
							if (x <= 0 || y <= 0 || x > edgeMatrix->getRows()
									|| y > edgeMatrix->getCols())
								continue;
							else {
								if (edgeMatrix->getAtPosition(x, y) >= high) {
									edgeMatrix->setAtPosition(i, j, 255);
									anyHigh = true;
									break;
								} else {
									if (edgeMatrix->getAtPosition(x, y) < high
											&& edgeMatrix->getAtPosition(x, y)
													>= low)
										anyBetween = true;
								}
							}
						}
						if (anyHigh)
							break;
					}
					if (!anyHigh && anyBetween) {
						for (int x = i - 2; x < i + 3; x++) {
							for (int y = j - 1; y < j + 3; y++) {
								if (x < 0 || y < 0 || x >= edgeMatrix->getRows()
										|| y >= edgeMatrix->getCols())
									continue;
								else {
									if (edgeMatrix->getAtPosition(x, y)
											>= high) {
										edgeMatrix->setAtPosition(i, j, 255);
										anyHigh = true;
										break;
									}
								}
							}
							if (anyHigh)
								break;
						}

					}
					if (!anyHigh)
						edgeMatrix->setAtPosition(i, j, 0);
				}
			}
		}
	}
	return edgeMatrix;
}

void cannyProcess(ptr_IntMatrix binaryImage, int lowThreshold, int highThreshold) {

	vector<vector<double> > filter = createGaussianFilter(1, 1);

	cout<<"\nFinished create the gaussian filter.\n";

	ptr_IntMatrix gaussianFilter = applyGaussianFilter(binaryImage, filter);

	cout<<"\nFinished apply the gaussian filter.\n";

	ptr_IntMatrix sobelFilter = sobelOperation(gaussianFilter);

	cout<<"\nFinished apply sobel filter.\n";

	ptr_IntMatrix nonMaxSuppress = nonMaxSuppression(sobelFilter);

	cout<<"Finished non maximum suppression.\n";

	ptr_IntMatrix thresh = doubleThreshold(nonMaxSuppress, lowThreshold, highThreshold);

	cout<<"Finished double threshold.\n";

	int i = 0, count = 0;
	ofstream of("output/edgeValues.txt");

	for (int r = 0; r < thresh->getRows(); r++) {
		for (int c = 0; c < thresh->getCols(); c++) {
			if (thresh->getAtPosition(r, c) == 255) {
				of << r << "\t" << c << "\n";
				count++;
				if (i < 10) {
					cout << "row: " << r << ", col: " << c << endl;
					i++;
				}
			}
		}

	}
	cout<<endl<<"Total point: "<< count;
	of.close();
	saveGrayJPG(thresh, thresh->getCols(), thresh->getRows(),
			"output/new_cannyyyyyy.jpg");
}
