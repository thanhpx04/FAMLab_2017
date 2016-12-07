/*
 * Analysis.h
 *
 *  Created on: Dec 7, 2016
 *      Author: linh
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

class Analysis
{
private:
	ptr_Treatments treatment;
public:
	Analysis();
	Analysis(ptr_Treatments);
	virtual ~Analysis();
	vector<ptr_Line> segment(Image sceneImage, int minDistance);
	ShapeHistogram geomHistogram(Image image, AngleAccuracy angleAcc, int cols);
	double bhattacharyyaDistance(Image sceneImage, AngleAccuracy angleAcc,
			int cols);
	PHoughTransform phtEntriesTable(Image image);
	vector<ptr_Point> estimatedLandmarks(Image sceneImage,AngleAccuracy acc, int cols, int templSize, int sceneSize);
};

#endif /* ANALYSIS_H_ */
