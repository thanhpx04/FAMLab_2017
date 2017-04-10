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
#include "Filters.h"

Matrix<double> getGaussianKernel(int ksize, double sigma)
{
	Matrix<double> kernel(ksize, ksize, 0.0);
	if (sigma <= 0)
	{
		sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8;
	}
	double mean = ksize / 2;
	double scale = (2 * sigma * sigma);
	double evalue;
	double sum = 0;
	for (int i = 0; i < ksize; i++)
	{
		for (int j = 0; j < ksize; j++)
		{
			evalue = 0.0;
			double r = pow(i - mean, 2) + pow(j - mean, 2);
			evalue = exp(-r / scale) / (scale * M_PI);
			kernel.setAtPosition(i, j, evalue);
			sum += evalue;
		}
	}
	// normalized
	for (int i = 0; i < ksize; i++)
	{
		for (int j = 0; j < ksize; j++)
		{
			kernel.setAtPosition(i, j, kernel.getAtPosition(i, j) / sum);
			cout << "\t" << kernel.getAtPosition(i, j);
		}
		cout << endl;
	}
	return kernel;
}

Matrix<int> gaussianBlur(Matrix<int> inputMatrix, Matrix<double> kernel)
{
	int rows = inputMatrix.getRows();
	int cols = inputMatrix.getCols();
	int begin = kernel.getRows()/2;
	Matrix<int> result(rows, cols);
	for (int r = begin; r < rows; r++)
	{
		for (int c = begin; c < cols; c++)
		{
			int value;
			if (r < 0)
			{
				if (c < 0)
				{
					value = inputMatrix.getAtPosition(0, 0);
				}
				else
				{
					value = inputMatrix.getAtPosition(0, c);
				}
			}
			else
			{
				if (c < 0)
				{
					value = inputMatrix.getAtPosition(r, 0);
				}
				else
				{
					value = inputMatrix.getAtPosition(r, c);
				}
			}

		}
	}
}
