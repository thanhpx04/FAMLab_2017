/*
 * PHoughTransform.h
 *
 *  Created on: Nov 30, 2016
 *      Author: linh
 */

#ifndef PHOUGHTRANSFORM_H_
#define PHOUGHTRANSFORM_H_

bool closetLine(ptr_Line line1, ptr_Line line2);
class PHoughTransform
{
private:
	ptr_Point refPoint;
	vector<ptr_PHTEntry> phtEntries;
	//ptr_IntMatrix accumulator;
public:
	PHoughTransform();
	virtual ~PHoughTransform();

	void setRefPoint(ptr_Point rpoint);
	ptr_Point getRefPoint();
	vector<ptr_PHTEntry> getPHTEntries();
	vector<ptr_PHTEntry> constructPHTTable(vector<ptr_Line> lines);
};

#endif /* PHOUGHTRANSFORM_H_ */
