/*
 * PCA.h
 *
 *  Created on: Feb 7, 2017
 *      Author: linh
 */

#ifndef PCA_H_
#define PCA_H_
Line principalAxis(ptr_IntMatrix gradMatrix, Point &cPoint);
double rotateDirection(Line refLine, Line objLine, double angle);

#endif /* PCA_H_ */
