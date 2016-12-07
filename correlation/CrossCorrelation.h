/*
 * CrossCorrelation.h
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */

#ifndef CROSSCORRELATION_H_
#define CROSSCORRELATION_H_
vector<ptr_Point> verifyLandmarks(Image mImage, Image sImage,
	vector<ptr_Point> manualLM, vector<ptr_Point> esLandmarks, int templSize,
	int sceneSize, double angleDiff, ptr_Point ePoint);

class CrossCorrelation
{
public:
	CrossCorrelation();
	virtual ~CrossCorrelation();
};

#endif /* CROSSCORRELATION_H_ */
