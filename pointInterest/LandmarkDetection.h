/*
 * LandmarkDetection.h
 *
 *  Created on: Dec 7, 2016
 *      Author: linh
 */

#ifndef LANDMARKDETECTION_H_
#define LANDMARKDETECTION_H_

class LandmarkDetection: public Treatments
{
public:
	LandmarkDetection();
	virtual ~LandmarkDetection();
	vector<ptr_Point> landmarksAutoDectect(Image sceneImage,AngleAccuracy acc, int cols, int templSize, int sceneSize);
};

#endif /* LANDMARKDETECTION_H_ */
