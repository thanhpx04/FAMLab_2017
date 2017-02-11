/*
 * PCA.cpp
 *
 *  Created on: Feb 7, 2017
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <float.h>
#include <string.h>
using namespace std;
#include "../utils/Converter.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"

#include "PCA.h"
double avgDistance(vector<Point> listPoints, Line axis)
{
	double totalDist = 0;
	size_t nPoints = listPoints.size();
	for (size_t j = 0; j < nPoints; j++)
	{
		Point pj = listPoints.at(j);
		double distance = axis.perpendicularDistance(pj);
		totalDist += distance;
	}
	return totalDist / (int) nPoints;
}

// compute centroid point from the gradient matrix of image
// output: centroid point and list points of edge.
Point centroidPoint(ptr_IntMatrix gradMatrix, vector<Point> &listPoints)
{
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();
	int totalX = 0, totalY = 0, count = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (gradMatrix->getAtPosition(r, c) != -1)
			{
				listPoints.push_back(Point(c, r));
				totalX += c;
				totalY += r;
				count++;
			}
		}
	}
	return Point(totalX / count, totalY / count);
}

Line principalAxis(ptr_IntMatrix gradMatrix, Point &cPoint)
{
	vector<Point> listOfPoints;
	cPoint = centroidPoint(gradMatrix, listOfPoints);
	Point sPoint;
	double minAvgDist = DBL_MAX;
	size_t nPoints = listOfPoints.size();
	for (size_t i = 0; i < nPoints; ++i)
	{
		Point pi = listOfPoints.at(i);
		Line l(cPoint, pi);
		double avgDist = avgDistance(listOfPoints, l);
		if (avgDist < minAvgDist)
		{
			minAvgDist = avgDist;
			sPoint.setX(pi.getX());
			sPoint.setY(pi.getY());
		}
	}

	return Line(cPoint, sPoint);
}
// negative for clockwise, positive for counterclockwise
// two lines have the same origin
double rotateDirection(Line refLine, Line objLine, double angle)
{
	int xnew = 0, ynew = 0;
	Point origin = refLine.getBegin();
	Point rpoint = objLine.getEnd();
	rotateAPoint(rpoint.getX(), rpoint.getY(), origin, angle, 1, xnew, ynew);
	double distance1 = refLine.perpendicularDistance(Point(xnew, ynew));
	xnew = ynew = 0;
	rotateAPoint(rpoint.getX(), rpoint.getY(), origin, -angle, 1, xnew, ynew);
	double distance2 = refLine.perpendicularDistance(Point(xnew, ynew));
	if (distance2 < distance1)
		angle = -angle;
	return angle;
}

