/*
 * Filters.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: linh
 */
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <float.h>

using namespace std;
#include "../imageModel/Point.h"
#include "../imageModel/Matrix.h"
#include "Canny.h"
#include "Filters.h"

Matrix<double> getGaussianKernel(int ksize, double sigma) {
	Matrix<double> kernel(ksize, ksize, 0.0);
	if (sigma <= 0) {
		sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8;
	}
	double mean = ksize / 2;
	double scale = (2 * sigma * sigma);
	double evalue;
	double sum = 0;
	for (int i = 0; i < ksize; i++) {
		for (int j = 0; j < ksize; j++) {
			evalue = 0.0;
			double r = pow(i - mean, 2) + pow(j - mean, 2);
			evalue = exp(-r / scale) / (scale * M_PI);
			kernel.setAtPosition(i, j, evalue);
			sum += evalue;
		}
	}
	// normalized
	for (int i = 0; i < ksize; i++) {
		for (int j = 0; j < ksize; j++) {
			kernel.setAtPosition(i, j, kernel.getAtPosition(i, j) / sum);
			//cout << "\t" << kernel.getAtPosition(i, j);
		}
		//cout << endl;
	}
	return kernel;
}

Matrix<int> gaussianBlur(Matrix<int> inputMatrix, Matrix<double> kernel) {
	int rows = inputMatrix.getRows();
	int cols = inputMatrix.getCols();
	int krows = kernel.getRows();
	int kcols = kernel.getCols();

	int begin = krows / 2;
	//Matrix<int> result(rows, cols);
	for (int r = -begin; r < rows - begin; r++) {
		for (int c = -begin; c < cols - begin; c++) {
			double sum = 0;
			for (int i = 0; i < krows; i++) {
				for (int j = 0; j < kcols; j++) {
					double kvalue = kernel.getAtPosition(i, j);
					double gValue =
							(i + r < 0 || j + c < 0 || i + r >= rows
									|| j + c >= cols) ?
									getBorderValue(&inputMatrix, i + r, j + c) :
									(double) inputMatrix.getAtPosition(i + r,
											j + c);
					sum += kvalue * gValue;
				}
			}
			inputMatrix.setAtPosition(r+begin,c+begin,sum);
		}
	}
	return inputMatrix;
}
