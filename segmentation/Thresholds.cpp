/*
 * Thresholds.cpp
 *
 *  Created on: Nov 26, 2016
 *      Author: linh
 */
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "Filters.h"

#include "Thresholds.h"

ptr_IntMatrix binaryThreshold(ptr_IntMatrix inputMatrix, int tValue,
	int maxValue)
{
	int rows = inputMatrix->getRows();
	int cols = inputMatrix->getCols();

	/*Matrix<double> kernel = getGaussianKernel(9,1);
	 *inputMatrix = gaussianBlur(*inputMatrix,kernel);*/

	ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (inputMatrix->getAtPosition(r, c) > tValue)
				result->setAtPosition(r, c, maxValue);
			else
				result->setAtPosition(r, c, 0);
		}
	}

	return result;
}

/*

 ptr_IntMatrix postProcess(ptr_IntMatrix binaryMatrix, int maxValue)
 {
 int rows = binaryMatrix->getRows();
 int cols = binaryMatrix->getCols();
 ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
 *result = *binaryMatrix;
 Point left(0, 0), right(0, 0);
 vector<Line> lines;
 for (int r = 1; r < rows - 1; r++)
 {
 for (int c = 1; c < cols - 1; c++)
 {
 left.setX(0);
 left.setY(0);
 right.setX(0);
 right.setY(0);
 lines.clear();
 int value = binaryMatrix->getAtPosition(r, c);
 if (value == maxValue && binaryMatrix->getAtPosition(r, c - 1) == 0)
 {
 // xac dinh diem dau tien
 left.setX(c - 1);
 left.setY(r);
 // xac dinh diem thu 2
 for (int k = c; k < cols; k++)
 {
 if (binaryMatrix->getAtPosition(r, k) == 0
 && binaryMatrix->getAtPosition(r, k - 1) == maxValue)
 {
 right.setX(k);
 right.setY(r);
 break;
 }
 }

 // do dong truoc do neu ko phai den thi ko xet nua ???

 if (right.getX() > left.getX())
 {
 for (int l = left.getX(); l <= right.getX(); l++)
 {
 if (binaryMatrix->getAtPosition(r - 1, l) == maxValue)
 {
 left.setX(0);
 left.setY(0);
 right.setX(0);
 right.setY(0);

 break;

 }
 }
 }
 if (left != 0 && right != 0)
 {
 bool inhole(true);
 int rnew = r;
 int clnew = 0, crnew = 0;
 lines.push_back(Line(left, right));
 do
 {
 rnew += 1;
 if (rnew == rows)
 {
 lines.clear();
 inhole = false;
 break;
 }
 clnew = 0;
 crnew = 0;
 inhole = false;
 if (binaryMatrix->getAtPosition(rnew, left.getX()) == maxValue)
 {
 for (int l = left.getX(); l > 0; l--)
 {
 if (binaryMatrix->getAtPosition(rnew, l) == 0)
 {
 clnew = l;
 break;
 }
 }
 }
 else
 {
 for (int l = left.getX(); l < cols; l++)
 {
 if (binaryMatrix->getAtPosition(rnew, l) == maxValue)
 {
 clnew = l - 1;
 break;
 }
 }
 }
 if (binaryMatrix->getAtPosition(rnew, right.getX()) == maxValue)
 {
 for (int m = right.getX(); m < cols; m++)
 {
 if (binaryMatrix->getAtPosition(rnew, m) == 0)
 {
 crnew = m;
 break;
 }
 }
 }
 else
 {
 for (int m = right.getX(); m > 0; m--)
 {
 if (binaryMatrix->getAtPosition(rnew, m) == maxValue)
 {
 crnew = m + 1;
 break;
 }
 }
 }
 for (int n = clnew; n <= crnew; n++)
 {
 if (n < cols && rnew < rows
 && binaryMatrix->getAtPosition(rnew, n) == maxValue)
 {
 inhole = true;
 break;
 }
 }
 if (clnew != 0 && crnew != 0)
 {
 left.setX(clnew);
 left.setY(rnew);
 right.setX(crnew);
 right.setY(rnew);
 lines.push_back(Line(left, right));
 }
 else
 {
 lines.clear();
 inhole = false;
 }

 }
 while (inhole);

 for (size_t li = 0; li < lines.size(); li++)
 {
 Line line = lines.at(li);
 if (line.getBegin().getY() == line.getEnd().getY())
 {
 for (int x = line.getBegin().getX(); x < line.getEnd().getX();
 x++)
 {
 result->setAtPosition(line.getBegin().getY(), x, 0);
 }
 }
 }
 lines.clear();
 }

 }
 }
 }

 return result;
 }
 */

ptr_IntMatrix postProcess(ptr_IntMatrix binaryMatrix, int maxValue)
{
	int rows = binaryMatrix->getRows();
	int cols = binaryMatrix->getCols();
	ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
	*result = *binaryMatrix;
	Point left(0, 0), right(0, 0);
	vector<Line> lines;

	for (int r = 1; r < rows - 1; r++)
	{
		for (int c = 1; c < cols - 1; c++)
		{
			left.setX(0);
			left.setY(0);
			right.setX(0);
			right.setY(0);
			lines.clear();
			int value = binaryMatrix->getAtPosition(r, c);
			int valuel = binaryMatrix->getAtPosition(r, c - 1);
			if (value == maxValue && valuel == 0)
			{
				bool beginHole = true;
				// xac dinh diem dau tien
				left.setX(c - 1);
				left.setY(r);
				if (left.getX() != 0)
				{
					// xac dinh diem thu 2
					for (int k = c; k < cols; k++)
					{
						if (binaryMatrix->getAtPosition(r, k) == 0
							&& binaryMatrix->getAtPosition(r, k - 1) == maxValue)
						{
							right.setX(k);
							right.setY(r);
							goto checkbghole;
						}
					}
				}
				// kiem tra hole

				checkbghole: if (left != 0 && right != 0 && right.getX() > left.getX())
				{
					// do dong truoc do

					if (r - 1 < 0 || r - 1 >= rows)
						beginHole = false;
					else
					{
						for (int l = left.getX() + 1; l <= right.getX() - 1; l++)
						{
							if (binaryMatrix->getAtPosition(r - 1, l) == maxValue)
							{
								beginHole = false;
								goto resetValues;
							}
						}
					}
					resetValues: if (beginHole == false)
					{
						left.setX(0);
						left.setY(0);
						right.setX(0);
						right.setY(0);
					}
				}
				if (left != 0 && right != 0 && beginHole)
				{
					bool inhole = true;
					int rnew = r + 1;
					int clnew = 0, crnew = 0;
					lines.push_back(Line(left, right));
					do
					{
						clnew = 0;
						crnew = 0;
						if (rnew < rows)
						{
							// check dong do
							inhole = false;
							for (int n = left.getX() + 1; n <= right.getX() - 1; n++)
							{
								if (n < cols && rnew < rows
									&& binaryMatrix->getAtPosition(rnew, n) == maxValue)
								{
									inhole = true;
									goto checkinhole;
								}
							}
							checkinhole: if (inhole)
							{
								if (binaryMatrix->getAtPosition(rnew, left.getX()) == maxValue)
								{
									for (int l = left.getX(); l <= right.getX(); l++)
									{
										if (binaryMatrix->getAtPosition(rnew, l) == 0)
										{
											clnew = l;
											break;
										}
									}
									if (clnew == 0)
									{
										for (int l = left.getX(); l > 0; l--)
										{
											if (binaryMatrix->getAtPosition(rnew, l) == 0)
											{
												clnew = l;
												goto checkright;
											}
										}
									}
								}
								checkright: if (binaryMatrix->getAtPosition(rnew, right.getX())
									== maxValue)
								{
									for (int m = right.getX(); m >= clnew; m--)
									{
										if (binaryMatrix->getAtPosition(rnew, m) == 0)
										{
											crnew = m;
											goto checkandpush;
										}
									}
									/*if (crnew == 0)
									 {
									 for (int m = right.getX(); m < cols; m++)
									 {
									 if (binaryMatrix->getAtPosition(rnew, m) == 0)
									 {
									 crnew = m;
									 goto checkandpush;
									 }
									 }
									 }*/
								}
								checkandpush: if (clnew != 0 && crnew != 0)
								{
									left.setX(clnew);
									left.setY(rnew);
									right.setX(crnew);
									right.setY(rnew);
									lines.push_back(Line(left, right));
								}
							}
						}
						else
						{
							lines.clear();
							inhole = false;
						}
						rnew++;

					}
					while (inhole);

					for (size_t li = 0; li < lines.size(); li++)
					{
						Line line = lines.at(li);
						if (line.getBegin().getY() == line.getEnd().getY())
						{
							for (int x = line.getBegin().getX(); x < line.getEnd().getX();
								x++)
							{
								binaryMatrix->setAtPosition(line.getBegin().getY(), x, 0);
							}
						}
					}
					lines.clear();
				}

			}
		}
	}

	return binaryMatrix;
}

