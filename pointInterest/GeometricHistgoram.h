/*
 * GeometricHistgoram.h
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */

#ifndef GEOMETRICHISTGORAM_H_
#define GEOMETRICHISTGORAM_H_

double bhattacharyyaMetric(ShapeHistogram refHist, ShapeHistogram sceneHist);

class GeometricHistgoram: public Treatments
{
private:
	ShapeHistogram gmHistogram(Image image, AngleAccuracy angleAcc, int cols);
public:
	GeometricHistgoram();
	virtual ~GeometricHistgoram();
	ShapeHistogram geomHistogram(AngleAccuracy angleAcc, int cols);
	double bhattacharyyaDistance(Image sceneImage, AngleAccuracy angleAcc,
		int cols);
};

#endif /* GEOMETRICHISTGORAM_H_ */
