/*
 * LBP.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include "LBP.h"

int lbpCalculate(ptr_IntMatrix grayImage, int rowIndex, int colIndex)
{
	int rows = grayImage->getRows();
	int cols = grayImage->getCols();
	int i = 0;
	int vIndex = grayImage->getAtPosition(rowIndex, colIndex);
	int lbpSum = 0;
	for (int r = rowIndex - 1; r <= rowIndex + 1; r++)
	{
		for (int c = colIndex - 1; c <= colIndex + 1; c++)
		{
			int value;
			if (r < 0 || c < 0 || r >= rows || c >= cols)
			{
				value = 0;
			}
			else
			{
				value = grayImage->getAtPosition(r, c);
			}
			if (r != rowIndex || c != colIndex)
			{
				int bValue;
				if (value >= vIndex)
					bValue = 1;
				else
					bValue = 0;
				lbpSum += (bValue * pow(2, i));
				i++;
			}
		}
	}
	return lbpSum;
}

ptr_IntMatrix LBPDescriptor(ptr_IntMatrix grayImage)
{
	int rows = grayImage->getRows();
	int cols = grayImage->getCols();
	ptr_IntMatrix result = new Matrix<int>(rows, cols, 0);
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int lbpValue = lbpCalculate(grayImage, r, c);
			result->setAtPosition(r, c, lbpValue);
		}
	}
	return result;
}

void LBPHistogram(ptr_IntMatrix lbpMatrix, double (&histogram)[256])
{
	int rows = lbpMatrix->getRows();
	int cols = lbpMatrix->getCols();
	//double histCount[256] =
	//{ };
	//hist = {};
	// compute the histogram
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int value = lbpMatrix->getAtPosition(r, c);
			histogram[value] += 1;
		}
	}

	// normalize the histogram
	int total = rows * cols;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] /= total;
	}
}
double measureDistance(double hist1[256], double hist2[256])
{
	double distance = 0.0, temp = 0.0;
	for (int i = 0; i < 256; ++i)
	{
		double v1 = hist1[i];
		double v2 = hist2[i];
		temp += pow(v1 - v2, 2.0);
	}
	distance = sqrt(temp);
	return distance;
}

void testLBPDescriptor(ptr_IntMatrix grayImage, vector<Point> landmarks,
	int sPatch)
{
	for (int i = 0; i < landmarks.size(); i++)
	{
		Point pi = landmarks.at(i);
		Matrix<int> patch = grayImage->extractPatch(sPatch, sPatch, pi.getY(),
			pi.getX(), 0);
		double hist1[256] =
		{ };
		LBPHistogram(&patch, hist1);
		for (int j = 0; j < landmarks.size(); j++)
		{
			double hist2[256] =
			{ };
			Point pk = landmarks.at(j);
			Matrix<int> patch2 = grayImage->extractPatch(sPatch, sPatch, pk.getY(),
				pk.getX(), 0);
			LBPHistogram(&patch2, hist2);
			double dist = measureDistance(hist1, hist2);
			cout << "\n Pair: " << i << " - " << j << ": " << dist << endl;
		}
	}
}

void testLBPDescriptor2Images(ptr_IntMatrix grayImage1,
	vector<Point> landmarks1, ptr_IntMatrix grayImage2, vector<Point> landmarks2,
	int sPatch)
{
	for (int i = 0; i < landmarks1.size(); i++)
	{
		Point pi = landmarks1.at(i);
		Matrix<int> patch = grayImage1->extractPatch(sPatch, sPatch, pi.getY(),
			pi.getX(), 0);
		double hist1[256] =
		{ };
		LBPHistogram(&patch, hist1);
		for (int j = 0; j < landmarks2.size(); j++)
		{
			double hist2[256] =
			{ };
			Point pk = landmarks2.at(j);
			Matrix<int> patch2 = grayImage2->extractPatch(sPatch, sPatch, pk.getY(),
				pk.getX(), 0);
			LBPHistogram(&patch2, hist2);
			double dist = measureDistance(hist1, hist2);
			cout << "\n Pair: " << i << " - " << j << ": " << dist << endl;
		}
	}
}
