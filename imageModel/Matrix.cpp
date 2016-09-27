/*
 * Matrix.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: linh
 */

 #include <iostream>
#include <vector>
using namespace  std;

#include "Point.h"
#include "Matrix.h"

/*
Matrix<T>::Matrix() {

}


Matrix<T>::~Matrix() {
}


template <typename T>
Matrix<T>::Matrix(int nrows, int ncols)
{
	rows = nrows;
	cols = ncols;
	
	// Initialization the matrix
	data.resize(nrows);
	for (int i = 0; i < nrows; ++i)
    	data[i].resize(ncols);
}

template <typename T>
void Matrix<T>::Init()
{
	data.resize(getRows());
	for (int i = 0; i < getRows(); ++i)
    	data[i].resize(getCols());
}

 

template <typename T>
void Matrix<T>::InitWithValue(T value)
{
	data.resize(getRows());
	for (int i = 0; i < getRows(); ++i)
    	data[i].resize(getCols(),value);	
}
*/
template <typename T>
T Matrix<T>::getAtPosition(int x, int y)
{
	return data[x][y];
}

template <typename T>
void Matrix<T>::setAtPosition(int x, int y, T value)
{
	data[x][y] = value;
}

template <typename T>
void Matrix<T>::printMatrix()
{
	for(int i =0; i < rows; i++)
	{
		for(int j =0;j < cols; j++)
		{
			cout <<data[i][j]<<"\t";
		}
		cout<<endl;
	}
}


template class Matrix<int>;
template class Matrix<ptr_Point>;

