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

/*class GHTInPoint
{
private:
	Point center;
	vector<REntry> entryTables;

public:
	GHTInPoint();
	virtual ~GHTInPoint();
};*/
RTable rTableConstruct(ptr_IntMatrix gradMatrix, Point center);
void houghSpace(ptr_IntMatrix gradMatrix, RTable rentries);
#endif /* GHTINPOINT_H_ */
