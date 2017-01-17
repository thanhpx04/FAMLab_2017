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

/*GHTInPoint::GHTInPoint()
 {
 // TODO Auto-generated constructor stub

 }

 GHTInPoint::~GHTInPoint()
 {
 // TODO Auto-generated destructor stub
 }*/
double angleVector(Point p1, Point p2, Point q1, Point q2)
{
	Point vp(p2.getX() - p1.getX(), p2.getY() - p1.getY());
	Point vq(q2.getX() - q1.getX(), q2.getY() - q1.getY());
	double lengthVP = sqrt(
			(double) (vp.getX() * vp.getX())
					+ (double) (vp.getY() * vp.getY()));
	double lengthVQ = sqrt(
			(double) (vq.getX() * vq.getX())
					+ (double) (vq.getY() * vq.getY()));

	double vpq = (double) (vp.getX() * vq.getX())
			+ (double) (vp.getY() * vq.getY());

	return acos(vpq / (lengthVP * lengthVQ));
	/*double vpq2 = (double) (vp.getX() * vq.getY())
	 - (double) (vp.getY() * vq.getX());
	 double result = atan2(vpq2, vpq) * 180 / M_PI;
	 if (result < 0)
	 result = 360 + result;
	 return result;*/
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

void houghSpace(ptr_IntMatrix gradMatrix, RTable rentries)
{
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();
	ptr_IntMatrix acc = new Matrix<int>(rows, cols, 0);

	int gradient = 0;
	REntry entry;
	int maxValue = 0, maxDIndex = 0, maxAIndex = 0;
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
							//if (aValue > maxValue)
							// {
							// maxValue = aValue;
							// maxDIndex = dindex;
							// maxAIndex = aindex;
							// }
						}
					}
				}
			}
		}
	}
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (acc->getAtPosition(r, c) >= maxValue)
			{
				maxValue = acc->getAtPosition(r, c);
				maxDIndex = c;
				maxAIndex = r;
				//cout << "\n Index: " << r << "\t" << c;
			}
		}
	}
	cout << "\n center: ";
	rentries.center.toString();
	cout << "\nMax Value:" << maxValue;
	cout << "\nMax X index:" << maxDIndex;
	cout << "\nMax Y index:" << maxAIndex << endl;
}
