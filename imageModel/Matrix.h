/*
 * Matrix.h
 *
 *  Created on: Sep 21, 2016
 *      Author: linh
 * This class describes an 2D matrix with type T
 */

#ifndef MATRIX_H_
#define MATRIX_H_

template <typename T>
class Matrix
{
  private:
    std::vector<std::vector<T> > data; // store the data
    int rows;			      // number of rows
    int cols;			      // number of columns
	void Init()
	{
		data.resize(getRows());
		for (int i = 0; i < getRows(); ++i)
			data[i].resize(getCols());
	}
	void InitWithValue(T value)
	{
		data.resize(getRows());
		for (int i = 0; i < getRows(); ++i)
			data[i].resize(getCols(),value);
	}
  public:
  	// basic constructor
    Matrix()
    {
		rows = 0;
		cols = 0;
    }

	// constructor with number of rows and column
    Matrix(int nrows, int ncols)
    {
		rows = nrows;
		cols = ncols;
		// Initialization the matrix
		Init();
    }

	// constructor with number of rows, columns and default value
	Matrix(int nrows, int ncols, T value)
	{
		rows = nrows;
		cols = ncols;
		InitWithValue(value);
	}
	Matrix(const Matrix<T>& tMatrix);
	Matrix<T>& operator=(const Matrix<T>& tMatrix);
    virtual ~Matrix() {}
    int getRows() { return rows; }
    int getCols() { return cols; }
    void setRows(int nrows) { rows = nrows; }
    void setCols(int ncols) { cols = ncols; }
    std::vector<std::vector<T> > getData()
	{
		return data;
	}
	void setData(std::vector<std::vector<T> > iData)
	{
		data = iData;
	}
    T getAtPosition(int, int);
	void setAtPosition(int, int, T);
	void printMatrix();
};

typedef Matrix<int>* ptr_Matrix;
typedef Matrix<ptr_Point> ptr_PointMatrix;
#endif /* MATRIX_H_ */
