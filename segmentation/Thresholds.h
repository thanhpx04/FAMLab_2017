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
ptr_RGBMatrix colorThreshold(ptr_RGBMatrix rgbImage, ptr_RGBMatrix colorHistogram);
//Thanh
void showIntMatrix(ptr_IntMatrix intMatrix);
void showRGBAMatrix(ptrRGBAMatrix rgbaMatrix);
ptr_IntMatrix copyGrayMatrix(ptr_IntMatrix grayMatrix, int fromRow, int toRow, int fromCol, int toCol);
ptr_RGBMatrix copyRGBMatrix(ptr_RGBMatrix RGBMatrix, int fromRow, int toRow, int fromCol, int toCol);
ptr_IntMatrix calculateHistogram(ptr_IntMatrix grayMatrix, float &medianHistogram, float &meanHistogram, float &thresholdValue);
int indexMaxInRangeOfGrayHistogram(ptr_IntMatrix grayHistogram, int from, int to);
int indexMinInRangeOfGrayHistogram(ptr_IntMatrix grayHistogram, int from, int to);
float calculateMAELabThreshold(float medianHistogram, float meanHistogram, ptr_IntMatrix grayHistogram);
void copySmallToBigIntMatrix(ptr_IntMatrix intSmallMatrix, ptr_IntMatrix intBigMatrix, int fromRow, int fromCol);
//========
#endif /* THRESHOLDS_H_ */
