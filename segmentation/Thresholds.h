/*
 * Thresholds.h
 *
 *  Created on: Nov 26, 2016
 *      Author: linh
 */

#ifndef THRESHOLDS_H_
#define THRESHOLDS_H_

ptr_IntMatrix binaryThreshold(ptr_IntMatrix inputMatrix, int tValue, int maxValue);
ptr_IntMatrix postProcess(ptr_IntMatrix binaryMatrix, int maxValue);
ptr_IntMatrix removeLeg(ptr_IntMatrix binaryImage);
void analysisHistogram(ptr_IntMatrix &histogram, int type, int sizeRegion);
#endif /* THRESHOLDS_H_ */
