/*
 * ProHoughTransform.h
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */

#ifndef PROHOUGHTRANSFORM_H_
#define PROHOUGHTRANSFORM_H_

ptr_PHTEntry matchingInScene(vector<ptr_PHTEntry> entryTable,
	vector<ptr_Line> sceneLines, int width, int height,
	vector<ptr_Line> &maxVector);
ptr_Point refPointInScene(ptr_PHTEntry entry, vector<ptr_Line> matchLines,
	double &angleDiff, vector<ptr_Point> refLandmarks, int width, int height);
vector<ptr_Point> phtLandmarks(vector<ptr_PHTEntry> entriesTable,
	ptr_Point refPoint, vector<ptr_Line> sceneLines, int width, int height,
	vector<ptr_Point> mLandmarks, double &angleDiff, ptr_Point &ePoint);


class ProHoughTransform: public Treatments
{
public:
	ProHoughTransform();
	virtual ~ProHoughTransform();
	PHoughTransform constructPHT();
	vector<ptr_Point> estimateLandmarks(Image sImage,
		double &angleDiff, ptr_Point &ePoint);
};

#endif /* PROHOUGHTRANSFORM_H_ */
