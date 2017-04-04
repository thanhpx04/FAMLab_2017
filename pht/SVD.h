/*
 * SVD.h
 *
 *  Created on: Feb 2, 2017
 *      Author: linh
 */

#ifndef SVD_H_
#define SVD_H_

vector<Point> test(Image modelImage, Image sceneImage, vector<Point> mnLandmarks);
vector<Edge> test2(Image image);
ptr_IntMatrix getContour(ptr_IntMatrix grayMatrix);
void binProjection(ptr_IntMatrix binImage, ptr_IntMatrix &hProjection,
	ptr_IntMatrix &vProjection);
#endif /* SVD_H_ */
