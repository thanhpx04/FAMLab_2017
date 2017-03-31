/*
 * Projection.h
 *
 *  Created on: Mar 22, 2017
 *      Author: linh
 */

#ifndef PROJECTION_H_
#define PROJECTION_H_

enum PROJECTION_TYPE
{
	Horizontal = 1,
	Vertical = 2
};
ptr_IntMatrix quantization(ptr_IntMatrix grayMatrix, int graylevel);

int* histogramProjection(ptr_IntMatrix binaryMatrix, PROJECTION_TYPE pType,
		int &size);
void analysisProjection(int* projection, int psize,int &leftLimit, int &rightLimit);
vector<Point> boundingBoxDetection(int* hProjection, int hSize, int* vProjection, int vSize);
vector<Point> boundingBoxDetection(ptr_IntMatrix grayMatrix);
#endif /* PROJECTION_H_ */
