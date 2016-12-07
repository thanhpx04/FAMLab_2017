/*
 * GeometricHistgoram.h
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */

#ifndef GEOMETRICHISTGORAM_H_
#define GEOMETRICHISTGORAM_H_

class GeometricHistgoram: public Treatments
{
public:
	GeometricHistgoram();
	virtual ~GeometricHistgoram();
	ShapeHistogram geomHistogram(Image image, AngleAccuracy angleAcc, int cols);
	double bhattacharyyaDistance(Image sceneImage,
		AngleAccuracy angleAcc, int cols);
};

#endif /* GEOMETRICHISTGORAM_H_ */
