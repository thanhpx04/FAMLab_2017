/*
 * Filters.h
 *
 *  Created on: Apr 7, 2017
 *      Author: linh
 */

#ifndef FILTERS_H_
#define FILTERS_H_
Matrix<double> getGaussianKernel(int ksize, double sigma);
Matrix<int> gaussianBlur(Matrix<int> inputMatrix, Matrix<double> kernel);
Matrix<int> RobertOperation(ptr_IntMatrix grayMatrix);
Matrix<int> SobelOperation(ptr_IntMatrix grayMatrix);
Matrix<int> postSobel(Matrix<int> sobelResult);
ptr_IntMatrix erode(ptr_IntMatrix binMatrix, int kernelSize);
ptr_IntMatrix dilate(ptr_IntMatrix binMatrix, int kernelSize);
ptr_IntMatrix openBinary(ptr_IntMatrix binMatrix, int kernelSize);
ptr_IntMatrix closeBinary(ptr_IntMatrix binMatrix, int kernelSize);
int thresholdOtsu(Matrix<int> sobelResult);
#endif /* FILTERS_H_ */
