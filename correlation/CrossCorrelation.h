/*
 * CrossCorrelation.h
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */

#ifndef CROSSCORRELATION_H_
#define CROSSCORRELATION_H_
vector<Point> verifyLandmarks(Image mImage, Image sImage,
		vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
		int sceneSize, double angleDiff, Point ePoint);
vector<Point> verifyLandmarks2(Image mImage, Image sImage,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize);
class CrossCorrelation
{
public:
	CrossCorrelation();
	virtual ~CrossCorrelation();
};

#endif /* CROSSCORRELATION_H_ */
