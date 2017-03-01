/*
 * DescriptorDistance.cpp
 *
 *  Created on: Feb 28, 2017
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
#include <float.h>
#include <iomanip>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Matrix.h"

#include "DescriptorDistance.h"

Matrix<double> createDescriptor(ptr_IntMatrix mImage, Point lefttop,
	Point rightbot, Matrix<double> &gradient)
{
	int rows = mImage->getRows();
	int cols = mImage->getCols();
	int rpatch = rightbot.getY() - lefttop.getY() + 1;
	int cpatch = rightbot.getX() - lefttop.getX() + 1;
	Matrix<double> orientation(rpatch, cpatch, 0.0);
	int location = 0, llocation = 0, rlocation = 0, alocation = 0, blocation = 0;
	int i = 0, j = 0;
	for (int r = lefttop.getY(); r <= rightbot.getY(); r++)
	{
		for (int c = lefttop.getX(); c <= rightbot.getX(); c++)
		{
			location = llocation = rlocation = alocation = blocation = 0;
			location = mImage->getAtPosition(r, c);
			if (r - 1 >= 0 && r - 1 < rows)
			{
				alocation = mImage->getAtPosition(r - 1, c);
			}
			if (r + 1 >= 0 && r + 1 < rows)
			{
				blocation = mImage->getAtPosition(r + 1, c);
			}
			if (c - 1 >= 0 && c - 1 < cols)
			{
				llocation = mImage->getAtPosition(r, c - 1);
			}
			if (c + 1 >= 0 && c + 1 < cols)
			{
				rlocation = mImage->getAtPosition(r, c + 1);
			}
			double d1 = (double) blocation - (double) alocation;
			double d2 = (double) rlocation - (double) llocation;
			if (d2 != 0)
			{
				double angle = atan(d1 / d2) * 180 / M_PI;
				if (angle < 0)
					angle += 360;
				orientation.setAtPosition(i, j, angle);

			}
			else
			{
				orientation.setAtPosition(i, j, 0.0);
			}
			double magnutide = (d2 * d2) + (d1 * d1);
			gradient.setAtPosition(i, j, sqrt(magnutide));
			j++;
		}
		i++;
		j = 0;
	}

	return orientation;
}
vector<double> orientHist4(Matrix<double> gradient, Matrix<double> orientation,
	int rbegin, int cbegin)
{
	vector<double> histograms;
	histograms.resize(8, 0.0);
	double grad = 0.0, orien = 0.0;
	for (int r = rbegin; r < rbegin + 4; r++)
	{
		for (int c = cbegin; c < cbegin + 4; c++)
		{
			grad = gradient.getAtPosition(r, c);
			orien = orientation.getAtPosition(r, c);
			int bin = ceil(orien / 45);
			bin = (bin == 0) ? 0 : (bin - 1);
			histograms.at(bin) += grad;
		}
	}
	return histograms;
}
vector<double> orientHist16(Matrix<double> gradient, Matrix<double> orientation)
{
	vector<double> histograms, hist;
	for (int r = 0; r < gradient.getRows(); r += 4)
	{
		for (int c = 0; c < gradient.getCols(); c += 4)
		{
			hist.resize(8, 0.0);
			hist = orientHist4(gradient, orientation, r, c);
			histograms.insert(histograms.end(), hist.begin(), hist.end());
		}
	}
	// normalize the vector (twice)
	double totalValue = 0;
	for (int i = 0; i < histograms.size(); i++)
	{
		totalValue += (histograms.at(i) * histograms.at(i));
	}
	totalValue = sqrt(totalValue);

	//  normalizing and theshold < 0.2
	double totalValue2 = 0.0;
	for (int i = 0; i < histograms.size(); i++)
	{
		histograms.at(i) /= totalValue;
		if (histograms.at(i) < 0.2)
			histograms.at(i) = 0;
		totalValue2 += (histograms.at(i) * histograms.at(i));
	}
	//re-normalize
	for (int i = 0; i < histograms.size(); i++)
	{
		histograms.at(i) /= totalValue2;
		//cout << "\t" << histograms.at(i);
	}
	// end normalize vector
	//cout << "\nSize of histogram: " << histograms.size() << endl;
	return histograms;
}
double l2Distance(vector<double> sourceTarget, vector<double> targetHist)
{
	double distance = 0;
	size_t size = sourceTarget.size();
	if (size == targetHist.size())
	{
		double temp = 0;
		for (size_t i = 0; i < size; i++)
		{
			temp += pow(sourceTarget.at(i) - targetHist.at(i), 2.0);
		}
		distance = sqrt(temp);
	}
	return distance;
}
Point createPatch(ptr_IntMatrix imageMatrix, int psize, Point center,
	Point &right)
{
	Point left(0, 0);
	right = left;
	int hsize = psize / 2;
	int cx = center.getX();
	int cy = center.getY();
	int rows = imageMatrix->getRows();
	int cols = imageMatrix->getCols();

	int lx = (cx - hsize) < 0 ? 0 : (cx - hsize);
	int ly = (cy - hsize) < 0 ? 0 : (cy - hsize);
	left.setX(lx);
	left.setY(ly);

	int rx = (cx + hsize - 1) >= cols ? cols - 1 : (cx + hsize - 1);
	int ry = (cy + hsize - 1) >= rows ? rows - 1 : (cy + hsize - 1);
	right.setX(rx);
	right.setY(ry);
	return left;
}

// sceneSize and templSize should be is multiple of 16

vector<Point> verifyDescriptors(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize)
{
	vector<Point> result;
	int width = model->getCols();
	int height = model->getRows();
	size_t limit = 0;
	if (manualLM.size() < esLandmarks.size())
		limit = manualLM.size();
	else
		limit = esLandmarks.size();
	Point mpi, epi;
	for (size_t i = 0; i < limit; i++)
	{
		Point mleft(0, 0), mright(0, 0), sleft(0, 0), sright(0, 0);
		epi = esLandmarks.at(i);
		mpi = manualLM.at(i);
		if (epi.getX() >= 0 && epi.getY() >= 0 && epi.getX() < width
			&& epi.getY() < height)
		{

			mleft = createPatch(model, templSize, mpi, mright);
			sleft = createPatch(scene, sceneSize, epi, sright);
			// calculate the histogram for model
			cout << "\nCreate patches..." << endl;
			Matrix<double> mgradient(mright.getY() - mleft.getY() + 1,
				mright.getX() - mleft.getX() + 1, 0.0);
			Matrix<double> mOrient = createDescriptor(model, mleft, mright,
				mgradient);
			vector<double> mHistogram = orientHist16(mgradient, mOrient);
			double minDistance = DBL_MAX, maxDistance = 0.0;
			Point minPoint(0, 0), maxPoint(0, 0);
			//
			for (int r = sleft.getY(); r < sright.getY(); r++)
			{
				for (int c = sleft.getX(); c < sright.getX(); c++)
				{
					Point p(c, r);
					Point ssleft(0, 0), ssright(0, 0);
					ssleft = createPatch(scene, templSize, p, ssright);
					Matrix<double> sgradient(ssright.getY() - ssleft.getY() + 1,
						ssright.getX() - ssleft.getX() + 1, 0.0);
					Matrix<double> sOrient = createDescriptor(model, ssleft, ssright,
						sgradient);
					vector<double> sHistogram = orientHist16(sgradient, sOrient);
					double distance = l2Distance(mHistogram, sHistogram);
					if (distance > maxDistance)
					{
						maxDistance = distance;
						maxPoint.setX(c);
						maxPoint.setY(r);
					}
					if (distance < minDistance)
					{
						minDistance = distance;
						minPoint.setX(c);
						minPoint.setY(r);
					}
				}
			}
			result.push_back(minPoint);
		}
	}
	return result;
}
