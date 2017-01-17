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
		(double) (vp.getX() * vp.getX()) + (double) (vp.getY() * vp.getY()));
	double lengthVQ = sqrt(
		(double) (vq.getX() * vq.getX()) + (double) (vq.getY() * vq.getY()));

	double vpq = (double) (vp.getX() * vq.getX())
		+ (double) (vp.getY() * vq.getY());
	double vpq2 = (double) (vp.getX() * vq.getY())
		- (double) (vp.getY() * vq.getX());
	//return acos(vpq / (lengthVP * lengthVQ)) * 180 / M_PI;
	double result = atan2(vpq2, vpq) * 180 / M_PI;
	if (result < 0)
		result = 360 + result;
	return result;
}
RTable rTableConstruct(ptr_IntMatrix gradMatrix, Point center)
{
	RTable rtable;
	rtable.center.setX(center.getX());
	rtable.center.setY(center.getY());
	rtable.entriesTable.resize(361);
	Point hPoint(rtable.center.getX() + 100, rtable.center.getY());
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
				pv.angle = angleVector(rtable.center, hPoint, rtable.center, pv.p);
				pv.distance = (Line(rtable.center, pv.p)).getLength();
				rentry = rtable.entriesTable.at((gd));
				(rtable.entriesTable.at((gd))).gradient = gd;
				(rtable.entriesTable.at((gd))).polarValues.push_back(pv);
				off << "\n" << roundToDegree(pv.angle) << "\t" << pv.distance;
			}
		}
	}
	off.close();
	return rtable;
}
RTable2 rTableConstruct2(ptr_IntMatrix gradMatrix, Point center)
{
	RTable2 rtable;
	rtable.center.setX(center.getX());
	rtable.center.setY(center.getY());
	rtable.entries.resize(361);
	Point hPoint(rtable.center.getX() + 100, rtable.center.getY());
	// in default, rtable includes 360 rows for 360 degrees of gradient directions.
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();

	int gd;
	REntry2 rentry;
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
				int angle = roundToDegree(angleVector(rtable.center, hPoint, rtable.center, pv.p));
				double distance = (Line(rtable.center, pv.p)).getLength();
				rentry = rtable.entries.at((angle));
				if(rentry.gradient == 0)
					(rtable.entries.at((angle))).gradient = angle;
				(rtable.entries.at((angle))).distances.push_back(distance);
				off << "\n" << angle << "\t" <<  distance;
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
	ptr_IntMatrix acc = new Matrix<int>(cols, 361, 0);

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
						int aindex = 0, dindex = 0, aValue = 0;
						for (int k = 0; k < polarValues.size(); k++)
						{
							PolarValue pvalue = polarValues.at(k);
							aindex = pvalue.angle;
							dindex = round(pvalue.distance);

							//cout<<"\n"<<aindex<<"\t"<<dindex;
							// increase the value in accumulator
							aValue = acc->getAtPosition(dindex, aindex);
							aValue += 1;
							acc->setAtPosition(dindex, aindex, aValue);
							/*if (aValue > maxValue)
							 {
							 maxValue = aValue;
							 maxDIndex = dindex;
							 maxAIndex = aindex;
							 }*/
						}
					}
				}
			}
		}
	}
	for (int r = 0; r < 361; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (acc->getAtPosition(c, r) >= maxValue)
			{
				maxValue = acc->getAtPosition(c, r);
				maxDIndex = c;
				maxAIndex = r;
				//cout << "\n Index: " << r << "\t" << c;
			}
		}
	}
	cout << "\nMax Value:" << maxValue;
	cout << "\nMax distance index:" << maxDIndex << "\t x = "
		<< maxDIndex * cos(maxAIndex * M_PI/180);
	cout << "\nMax angle index:" << maxAIndex << "\t y = "
		<< maxDIndex * sin(maxAIndex * M_PI/180) << endl;
}
void houghSpace2(ptr_IntMatrix gradMatrix, RTable2 rentries)
{
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();
	Point hCenter(cols/2,rows/2);
	Point hPoint(hCenter.getX() + 100, hCenter.getY());

	ptr_IntMatrix acc = new Matrix<int>(cols, 361, 0);

	int gradient = 0;
	REntry2 entry;
	int maxValue = 0, maxDIndex = 0, maxAIndex = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			gradient = gradMatrix->getAtPosition(r, c);
			if (gradient != -1)
			{
				Point p(c,r);
				double g = roundToDegree(angleVector(hCenter,hPoint,hCenter,p));

				entry = rentries.entries.at(g);
				if (entry.gradient == g)
				{
					vector<double> distances = entry.distances;
					if (distances.size() > 0)
					{
						//PolarValue pvalue;
						int aindex = 0, dindex = 0, aValue = 0;
						for (int k = 0; k < distances.size(); k++)
						{
							double pvalue = distances.at(k);
							aindex = entry.gradient;
							dindex = round(pvalue);

							aValue = acc->getAtPosition(dindex, aindex);
							aValue += 1;
							acc->setAtPosition(dindex, aindex, aValue);
						}
					}
				}
			}
		}
	}
	for (int r = 0; r < 361; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (acc->getAtPosition(c, r) >= maxValue)
			{
				maxValue = acc->getAtPosition(c, r);
				maxDIndex = c;
				maxAIndex = r;
				//cout << "\n Index: " << r << "\t" << c;
			}
		}
	}
	cout << "\nMax Value:" << maxValue;
	cout << "\nMax distance index:" << maxDIndex << "\t x = "
		<< maxDIndex * cos(maxAIndex * M_PI/180);
	cout << "\nMax angle index:" << maxAIndex << "\t y = "
		<< maxDIndex * sin(maxAIndex * M_PI/180) << endl;
}
