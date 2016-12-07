/*
 * Treatments.h
 *
 *  Created on: Dec 2, 2016
 *      Author: linh
 */

#ifndef TREATMENTS_H_
#define TREATMENTS_H_

class Treatments
{
protected:
	Image refImage;
public:
	Treatments();
	virtual ~Treatments();
	void setRefImage(Image);
	Image getRefImage();

	//virtual vector<ptr_Line> segment(int minDistance) = 0;
	//virtual ShapeHistogram geomHistogram(AngleAccuracy angleAcc, int cols);

};

#endif /* TREATMENTS_H_ */
