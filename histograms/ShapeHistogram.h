/*
 * LocalHistogram.h
 *
 *  Created on: Oct 18, 2016
 *      Author: linh
 */

#ifndef SHAPEHISTOGRAM_H_
#define SHAPEHISTOGRAM_H_

/*class LocalHistogram {
private:
	vector<GFeature> listFeatures;
public:
	LocalHistogram();
	virtual ~LocalHistogram();
};*/
enum AngleAccuracy {
	HaftDegree = 0,
	Degree = 1,
	TwoTimeDegree = 2,
	FourTimeDegree = 4,
	SixTimeDegree = 6,
	TwelveTimeDegree = 12,
	SixtyTimeDegree = 60
};
struct GFeature {
	double angle;
	double dmin;
	double dmax;
};

struct LocalHistogram {
	vector<GFeature> features;
	double maxDistance;
};

vector<LocalHistogram> constructPGH(vector<ptr_Line> listOfLines,
		double &maxDistance);



ptr_IntMatrix shapeHistogram(Image grayImage, AngleAccuracy angleAcc, int cols);
double bhattacharyyaDistance(Image refImage, Image sceneImage,AngleAccuracy angleAcc, int cols);
#endif /* LOCALHISTOGRAM_H_ */
