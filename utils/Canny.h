/*
 * Canny.h
 *
 *  Created on: Oct 9, 2016
 *      Author: linh
 */

#ifndef CANNY_H_
#define CANNY_H_

/*
 * Input: - gray Matrix of image
 * 		  - lower threshold value
 * 		  - upper threshold value
 * Output: - matrix (in binary) contains the edges of image.
 */
vector<vector<ptr_Point> > canny(ptr_IntMatrix inputImg, int lowerThreshold,
		int upperThreshold);

#endif /* CANNY_H_ */
