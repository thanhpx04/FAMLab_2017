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
#endif /* FILTERS_H_ */
