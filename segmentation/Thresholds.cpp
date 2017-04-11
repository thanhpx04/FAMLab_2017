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

#include "Thresholds.h"

ptr_IntMatrix binaryThreshold(ptr_IntMatrix inputMatrix, int tValue,
	int maxValue)
{
	int rows = inputMatrix->getRows();
	int cols = inputMatrix->getCols();

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
					bool beginHole = true;
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
				if (left != 0 && right != 0)
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
									for (int l = left.getX(); l > 0; l--)
									{
										if (binaryMatrix->getAtPosition(rnew, l) == 0)
										{
											clnew = l;
											goto checkright;
										}
									}
								}
								checkright: if (binaryMatrix->getAtPosition(rnew, right.getX())
									== maxValue)
								{
									for (int m = right.getX(); m > left.getX(); m--)
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

ptr_IntMatrix removeLeg(ptr_IntMatrix binaryImage)
{
	int rows = binaryImage->getRows();
	int cols = binaryImage->getCols();
	int hcols = cols / 2;
	ptr_IntMatrix result(binaryImage);
	Point left(0, 0), right(0, 0);
	// remove left
	for (int r = 0; r < rows; r++)
	{
		bool first = true;
		for (int c = 0; c < hcols; c++)
		{
			if (first && result->getAtPosition(r, c) == 0
				&& (result->getAtPosition(r, c - 1) == 255 || c - 1 < 0))
			{
				left.setX(c - 1);
				left.setY(r);
				for (int k = c; k < hcols; k++)
				{
					if (result->getAtPosition(r, k) == 255)
					{
						right.setX(k);
						right.setY(r);
						break;
					}
				}
				if (left != 0 && right != 0)
				{
					for (int l = left.getX(); l < right.getX(); l++)
					{
						result->setAtPosition(r, l, 255);
					}
					left.setX(0);
					left.setY(0);
					right.setX(0);
					right.setY(0);
					first = false;
				}
			}
			if (!first)
				break;
		}
	}
	// remove right
	for (int r = 0; r < rows; r++)
	{
		bool first = true;
		for (int c = cols - 1; c > hcols; c--)
		{
			if (first && result->getAtPosition(r, c) == 0
				&& (result->getAtPosition(r, c - 1) == 255 || c - 1 < 0))
			{
				left.setX(c - 1);
				left.setY(r);
				for (int k = c; k < hcols; k++)
				{
					if (result->getAtPosition(r, k) == 255)
					{
						right.setX(k);
						right.setY(r);
						break;
					}
				}
				if (left != 0 && right != 0)
				{
					for (int l = left.getX(); l < right.getX(); l++)
					{
						result->setAtPosition(r, l, 255);
					}
					left.setX(0);
					left.setY(0);
					right.setX(0);
					right.setY(0);
					first = false;
				}
			}
			if (!first)
				break;
		}
	}
	return result;
}

void analysisHistogram(ptr_IntMatrix &histogram, int type, int sizeRegion) // type = 0 - horizontal, 1 - vertical
{
	int rows = histogram->getRows();
	int cols = histogram->getCols();
	int cs = 0, ls = 0;
	int arrSize = 0;
	if (type == 0)
	{
		arrSize = rows;
		ls = rows;
		cs = cols;
	}
	else
	{
		arrSize = cols;
		ls = cols;
		cs = rows;
	}
	int array[arrSize];
	for (int r = 0; r < ls; r++)
	{
		int count = 0;
		for (int c = 0; c < cs; c++)
		{
			if (type == 0)
			{
				if (histogram->getAtPosition(r, c) == 0)
					count++;
			}
			else
			{
				if (histogram->getAtPosition(c, r) == 0)
					count++;
			}
		}
		array[r] = count;
		cout << "\t" << count;
	}
	// analysis the projection
	vector<int> peakIndex;
	vector<int> valleyIndex;
	if (sizeRegion % 2 == 0)
		sizeRegion += 1;
	int hregion = sizeRegion / 2;
	int peaks = 0, valley = 0;
	for (int i = hregion; i < arrSize - hregion; i++)
	{
		int valuei = array[i];
		int count1 = 0, count2 = 0;
		for (int k = i - hregion; k <= i + hregion; k++)
		{
			if (k != i)
			{
				int valuek = 0;
				if (k < 0 || k >= arrSize)
				{
					valuek = 0;
				}
				else
				{
					valuek = array[k];
				}
				if (valuek < valuei)
				{
					count1++;
				}
				if (valuek > valuei)
				{
					count2++;
				}
			}
		}
		if (count1 == 0 && count2 == sizeRegion - 1)
		{
			valley++;
			valleyIndex.push_back(i);
		}
		if (count1 == sizeRegion - 1 && count2 == 0)
		{
			peaks++;
			peakIndex.push_back(i);
		}
	}
	cout << endl << "Number of peaks: " << peaks << endl;
	cout << endl << "Number of valleys: " << valley << endl;
	int maxDistance = 0, maxDistance2 = 0;
	int index = 0, index2 = 0;
	for (int i = 1; i < valleyIndex.size(); i++)
	{
		/*if (valleyIndex.at(i) < rows / 2)
		 {
		 int v1 = array[valleyIndex.at(i - 1)];
		 int v2 = array[valleyIndex.at(i)];
		 if (abs(v2 - v1) > maxDistance)
		 {
		 maxDistance = abs(v2 - v1);
		 index = i - 1;
		 }
		 }
		 else
		 {
		 int v1 = array[valleyIndex.at(i - 1)];
		 int v2 = array[valleyIndex.at(i)];
		 if (abs(v2 - v1) > maxDistance)
		 {
		 maxDistance2 = abs(v2 - v1);
		 index2 = i - 1;
		 }
		 }*/
		for (int r = 0; r < cs; r++)
		{
			histogram->setAtPosition(valleyIndex.at(i), r, 0);
		}
	}
	/*for (int r = 0; r < cs; r++)
	 {
	 histogram->setAtPosition(valleyIndex.at(index), r, 0);
	 histogram->setAtPosition(valleyIndex.at(index2), r, 0);
	 }*/
	/*for (int i = 0; i < peakIndex.size(); i++)
	 {
	 for (int r = 0; r < rows; r++)
	 {
	 histogram->setAtPosition(r, peakIndex.at(i), 0);
	 }
	 }*/
}

