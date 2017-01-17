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
struct REntry2
{
	int gradient;
	vector<double> distances;
};
struct RTable2
{
	Point center;
	vector<REntry2> entries;
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
RTable2 rTableConstruct2(ptr_IntMatrix gradMatrix, Point center);
void houghSpace2(ptr_IntMatrix gradMatrix, RTable2 rentries);
#endif /* GHTINPOINT_H_ */
