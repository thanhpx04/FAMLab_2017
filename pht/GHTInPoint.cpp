/*
 * GHTInPoint.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include "../utils/Converter.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "GHTInPoint.h"

double angleVector(Point p1, Point p2, Point q1, Point q2)
{
	Point vp(p2.getX() - p1.getX(), p2.getY() - p1.getY());
	Point vq(q2.getX() - q1.getX(), q2.getY() - q1.getY());

	double lengthVP = sqrt(
		(double) (vp.getX() * vp.getX()) + (double) (vp.getY() * vp.getY()));
	double lengthVQ = sqrt(
		(double) (vq.getX() * vq.getX()) + (double) (vq.getY() * vq.getY()));

	double vpq = (double) (vp.getX() * vq.getX())
		+ (double) (vp.getY() * vq.getY());

	return acos(vpq / (lengthVP * lengthVQ));
}
RTable rTableConstruct(ptr_IntMatrix gradMatrix, Point center)
{
	RTable rtable;
	rtable.center.setX(center.getX());
	rtable.center.setY(center.getY());
	rtable.entriesTable.resize(361);

	// in default, rtable includes 360 rows for 360 degrees of gradient directions.
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();

	int gd;
	REntry rentry;
	ofstream off("rtable.txt");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			gd = gradMatrix->getAtPosition(r, c);
			if (gd != -1)
			{
				PolarValue pv;
				pv.p.setX(c);
				pv.p.setY(r);
				Point hPoint(pv.p.getX() + 100, pv.p.getY());
				pv.angle = angleVector(pv.p, hPoint, pv.p, rtable.center);
				pv.distance = (Line(rtable.center, pv.p)).getLength();
				rentry = rtable.entriesTable.at((gd));

				(rtable.entriesTable.at((gd))).gradient = gd;
				(rtable.entriesTable.at((gd))).polarValues.push_back(pv);
				off << "\n" << pv.angle << "\t" << pv.distance;
			}
		}
	}
	off.close();
	return rtable;
}

Point houghSpace(ptr_IntMatrix gradMatrix, RTable rentries)
{
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();
	ptr_IntMatrix acc = new Matrix<int>(rows, cols, 0);

	int gradient = 0;
	REntry entry;
	int maxValue = 0, maxXIndex = 0, maxYIndex = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			gradient = gradMatrix->getAtPosition(r, c);
			if (gradient != -1)
			{
				entry = rentries.entriesTable.at(gradient);
				if (entry.gradient == gradient)
				{
					vector<PolarValue> polarValues = entry.polarValues;
					if (polarValues.size() > 0)
					{
						//PolarValue pvalue;
						double avalue = 0, dvalue = 0;
						int xindex = 0, yindex = 0, accValue = 0;
						for (int k = 0; k < polarValues.size(); k++)
						{
							PolarValue pvalue = polarValues.at(k);
							avalue = pvalue.angle;
							dvalue = pvalue.distance;

							xindex = c + (dvalue * cos(avalue));
							yindex = r + (dvalue * sin(avalue));
							//cout << "\n" << xindex << "\t" << yindex;
							if (xindex < cols && yindex < rows)
							{
								// increase the value in accumulator
								accValue = acc->getAtPosition(yindex, xindex);
								accValue += 1;
								acc->setAtPosition(yindex, xindex, accValue);
							}
							if (accValue > maxValue && xindex > 0 && xindex < cols
								&& yindex > 0 && yindex < rows)
							{
								maxValue = accValue;
								maxXIndex = xindex;
								maxYIndex = yindex;
							}
						}
					}
				}
			}
		}
	}
	cout << "\n center: ";
	rentries.center.toString();
	cout << "\nMax Value:" << maxValue;
	cout << "\nMax X index:" << maxXIndex;
	cout << "\nMax Y index:" << maxYIndex << endl;
	return Point(maxXIndex, maxYIndex);
}

vector<Point> detectLandmarks(Point refPoint, Point ePoint,
	vector<Point> mlandmarks, double &angle)
{
	int dx = abs(refPoint.getX() - ePoint.getX());
	int dy = abs(refPoint.getY() - ePoint.getY());
	//double diffangle = 0;
	angle = 0;
	if (dx > dy)
	{
		Point c(refPoint.getX() + 10, refPoint.getY());
		angle = angleVector(refPoint, c, refPoint, ePoint);
	}
	else
	{ // dy > dx
		Point c(refPoint.getX(), refPoint.getY() - 10);
		angle = angleVector(refPoint, c, refPoint, ePoint);
	}
	angle = angle * 180 / M_PI;
	if (angle > 90)
		angle = 180 - angle;
	cout << "\nAngle difference: " << angle;

	vector<Point> esLandmarks;
	Point mlm;
	int deltaX = 0, deltaY = 0, xn = 0, yn = 0;
	for (size_t i = 0; i < mlandmarks.size(); i++)
	{
		mlm = mlandmarks.at(i);
		deltaX = refPoint.getX() - mlm.getX();
		deltaY = refPoint.getY() - mlm.getY();

		xn = ePoint.getX() - deltaX;
		yn = ePoint.getY() - deltaY;
		esLandmarks.push_back(Point(xn, yn));
	}
	return esLandmarks;
}

Point centroidPoint(ptr_IntMatrix gradMatrix)
{
	int gd = 0;
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();
	int totalX = 0, totalY = 0, count = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			gd = gradMatrix->getAtPosition(r, c);
			if (gd != -1)
			{
				totalX += c;
				totalY += r;
				count++;
			}
		}
	}
	return Point(totalX/count,totalY/count);
}
