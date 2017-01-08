/*
 * Matrix.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: linh
 */

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "Point.h"

#include "Matrix.h"

template<typename T>
Matrix<T>::Matrix()
{
	rows = 0;
	cols = 0;
}
template<typename T>
Matrix<T>::Matrix(int nrows, int ncols)
{
	rows = nrows;
	cols = ncols;
	// Initialization the matrix
	Init();
}
template<typename T>
Matrix<T>::Matrix(int nrows, int ncols, T value)
{
	rows = nrows;
	cols = ncols;
	InitWithValue(value);
}
template<typename T>
Matrix<T>::Matrix(const Matrix<T> &tMatrix)
{
	this->rows = tMatrix.rows;
	this->cols = tMatrix.cols;
	Init();
	this->data = tMatrix.data;
}
template<typename T>
void Matrix<T>::Init()
{
	data.resize(getRows());
	for (int i = 0; i < getRows(); ++i)
		data[i].resize(getCols());
}

template<typename T>
void Matrix<T>::InitWithValue(T value)
{
	data.resize(getRows());
	for (int i = 0; i < getRows(); ++i)
		data[i].resize(getCols(), value);
}

template<typename T>
T Matrix<T>::getAtPosition(int r, int c)
{
	return data[r][c];
}

template<typename T>
void Matrix<T>::setAtPosition(int r, int c, T value)
{
	data[r][c] = value;
}

template<typename T>
void Matrix<T>::printMatrix()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//cout << data[i][j] << "\t";
		}
		cout << endl;
	}
}
template<typename T>
void Matrix<T>::operator=(const Matrix<T> &tMatrix)
{
	rows = tMatrix.rows;
	cols = tMatrix.cols;
	Init();
	data = tMatrix.data;
}
template class Matrix<int> ;
template class Matrix<double> ;
template class Matrix<RGB> ;

