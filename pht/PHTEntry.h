/*
 * PHTEntry.h
 *
 *  Created on: Nov 30, 2016
 *      Author: linh
 */

#ifndef PHTENTRY_H_
#define PHTENTRY_H_
struct HoughSpace
{
	double distance;
	double angle;
};
class PHTEntry
{
private:
	ptr_Line rLine; // reference line
	ptr_Line oLine; // object line
	vector<HoughSpace> listHoughSpace;
public:
	PHTEntry();
	virtual ~PHTEntry();
	vector<HoughSpace> getListHoughSpace();
	void setListHoughSpace(vector<HoughSpace> listHoughSpace);
	ptr_Line getObjLine();
	void setObjLine(ptr_Line line);
	ptr_Line getRefLine();
	void setRefLine(ptr_Line line);
	void addHoughSpace(HoughSpace hSpace);
};
typedef PHTEntry* ptr_PHTEntry;
#endif /* PHTENTRY_H_ */
