/*
 * GHTInPoint.h
 *
 *  Created on: Jan 17, 2017
 *      Author: linh
 */

#ifndef GHTINPOINT_H_
#define GHTINPOINT_H_
struct PolarValue
{
	Point p;
	double angle;
	double distance;
};

struct REntry
{
	double gradient;
	vector<PolarValue> polarValues;
};
struct RTable
{
	Point center;
	vector<REntry> entriesTable;
};
RTable rTableConstruct(ptr_IntMatrix gradMatrix, Point center);
Point houghSpace(ptr_IntMatrix gradMatrix, RTable rentries);
vector<Point> detectLandmarks(Point refPoint, Point ePoint,
	vector<Point> mlandmarks,double &angle);
#endif /* GHTINPOINT_H_ */
