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
			double d1 = (double) blocation - (double)alocation;
			double d2 = (double) rlocation - (double)llocation;
			orientation.setAtPosition(r,c,atan(d1/d2)* 180 / M_PI);
			double magnutide = (d2 * d2) + (d1 * d1);
			gradient.setAtPosition(r,c,sqrt(magnutide));
		}
	}

	return orientation;
}
