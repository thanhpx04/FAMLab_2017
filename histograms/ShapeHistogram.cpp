/*
 * LocalHistogram.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;

#include "../imageModel/Matrix.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Image.h"

// the minutes per bin corresponds with angle accuracy
const int m_bin_30 = 120;
const int m_bin_60 = 60; // each bin is
const int m_bin_120 = 30;
const int m_bin_240 = 15;
const int m_bin_360 = 10;
const int m_bin_720 = 5;
const int m_bin_3600 = 1;
#include "ShapeHistogram.h"

ShapeHistogram::ShapeHistogram()
{
}
ShapeHistogram::~ShapeHistogram()
{
}
double ShapeHistogram::getEntries()
{
	return totalEntries;
}
ptr_IntMatrix ShapeHistogram::getMatrix()
{
	return matrix;
}

GFeature pairwiseHistogram(ptr_Line refLine, ptr_Line objLine)
{
	GFeature pgh; // = GFeature();
	pgh.angle = refLine->angleLines(*objLine);

	double distance1 = refLine->perpendicularDistance(objLine->getBegin());
	double distance2 = refLine->perpendicularDistance(objLine->getEnd());
	pgh.dmin = ((distance1 < distance2 ? distance1 : distance2));
	pgh.dmax = ((distance1 < distance2 ? distance2 : distance1));
	return pgh;
}

double addGFeature(LocalHistogram &lhist, GFeature feature)
{
	lhist.features.push_back(feature);
	if (feature.dmax > lhist.maxDistance)
	{
		lhist.maxDistance = feature.dmax;
		return feature.dmax;
	}
	return lhist.maxDistance;
}

vector<LocalHistogram> ShapeHistogram::constructPGH(
	vector<ptr_Line> listOfLines)
{
	vector<LocalHistogram> result;
	double mDistance = 0;
	for (size_t t = 0; t < listOfLines.size(); t++)
	{
		ptr_Line refLine = listOfLines.at(t);
		LocalHistogram lh;
		for (size_t i = 0; i < listOfLines.size(); i++)
		{
			if (t != i)
			{
				ptr_Line objLine = listOfLines.at(i);
				GFeature pgh = pairwiseHistogram(refLine, objLine);
				addGFeature(lh, pgh);
			}
		}
		result.push_back(lh);
		if (lh.maxDistance > mDistance)
			mDistance = lh.maxDistance;
	}
	max_distance = mDistance;
	listLocalHistogram = result;

	return result;
}
int heightOfAngleAxis(AngleAccuracy angleAcc)
{
	if (angleAcc == HaftDegree)
		return 90;
	return angleAcc * 180;
}

int distanceOffset(double maxDistance, double distance, int cols)
{
	double binSize = maxDistance / cols;
	double bin = round(distance / binSize);
	return (bin > 0) ? (bin - 1) : bin;
}
int convertAngleToMinute(double angle)
{
	double degree;
	modf(angle, &degree);
	int minute = (angle - degree) * 60;
	int second = (angle - degree - minute / 60) * 3600;
	if (second >= 30)
		minute += 1;
	return (degree * 60) + minute;

}
int angleOffset(double angle, AngleAccuracy angleAcc)
{
	int m = m_bin_60;
	switch (angleAcc)
	{
	case HaftDegree:
		m = m_bin_30;
		break;
	case Degree:
		m = m_bin_60;
		break;
	case TwoTimeDegree:
		m = m_bin_120;
		break;
	case FourTimeDegree:
		m = m_bin_240;
		break;
	case SixTimeDegree:
		m = m_bin_360;
		break;
	case TwelveTimeDegree:
		m = m_bin_720;
		break;
	case SixtyTimeDegree:
		m = m_bin_3600;
		break;
	default:
		m = m_bin_60;
		break;
	}
	int minutes = convertAngleToMinute(angle);
	double bins = minutes / m;
	double bin = 0;
	double mod = modf(bins, &bin);
	if (mod > 0)
		bin += 1;
	return bin;
}

ptr_IntMatrix ShapeHistogram::constructPGHMatrix(
	vector<LocalHistogram> localHists, AngleAccuracy angleAcc, int cols)
{
	ofstream of("/home/linh/Desktop/compare/ShapeHistogram.txt");
	int rows = heightOfAngleAxis(angleAcc);
	double entries = 0;
	ptr_IntMatrix result = new Matrix<int>(rows, cols, 0);
	for (size_t t = 0; t < localHists.size(); t++)
	{
		LocalHistogram lh = localHists.at(t);
		vector<GFeature> lsFeatures = lh.features;
		for (size_t i = 0; i < lsFeatures.size(); i++)
		{
			GFeature feature = lsFeatures.at(i);
			int dmin = distanceOffset(max_distance, feature.dmin, cols);
			int dmax = distanceOffset(max_distance, feature.dmax, cols);
			int rowindex = angleOffset(feature.angle, angleAcc);
			if (!isnan(rowindex))
			{
				entries += (dmax - dmin) +1;
				for (int k = dmin; k <= dmax; k++)
				{
					if (rowindex >= 0 && k < cols)
					{
						int value = result->getAtPosition(rowindex, k);
						result->setAtPosition(rowindex, k, value + 1);
						//entries++;
					}
				}
			}
			of << feature.dmin << "\t" << feature.dmax <<"\t"<<rowindex<<"\t"<<feature.angle<<"\n";
		}
		//break;
	}
	totalEntries = entries;
	matrix = result;
	of.close();
	cout<<"\nTotal entries: "<<totalEntries;
	return result;
}

double sizeOfPGHMatrix(ptr_IntMatrix pghMatrix)
{
	double entries = 0;
	for (int r = 0; r < pghMatrix->getRows(); r++)
	{
		for (int c = 0; c < pghMatrix->getCols(); c++)
		{
			entries += pghMatrix->getAtPosition(r, c);
		}
	}
	return entries;
}

double bhattacharyyaMetric(ShapeHistogram refHist, ShapeHistogram sceneHist)
{
	double ref_size = refHist.getEntries();
	double scene_size = sceneHist.getEntries();
	double distance = 0;
	ptr_IntMatrix refMatrix = refHist.getMatrix();
	ptr_IntMatrix sceneMatrix = sceneHist.getMatrix();

	for (int r = 0; r < refMatrix->getRows(); r++)
	{
		for (int c = 0; c < refMatrix->getCols(); c++)
		{
			double value_1 = sqrt(refMatrix->getAtPosition(r, c) / ref_size);
			double value_2 = sqrt(sceneMatrix->getAtPosition(r, c) / scene_size);
			distance += value_1 * value_2;
		}
	}
	return distance;
}
//================================================================= Public method ===================================
// construct the geometric histogram of image
ShapeHistogram ShapeHistogram::shapeHistogram(Image grayImage,
	AngleAccuracy angleAcc, int cols)
{
	ShapeHistogram geoHistogram;
	vector<ptr_Line> lines = grayImage.getApproximateLines(3);
	vector<LocalHistogram> localHistogram = geoHistogram.constructPGH(lines);
	geoHistogram.constructPGHMatrix(localHistogram, angleAcc, cols);

	ofstream of("/home/linh/Desktop/compare/pp_ShapeHistogram.txt");

	ptr_IntMatrix matrix = geoHistogram.getMatrix();
	for (int r = 0; r < matrix->getRows(); r++)
	{
		for (int c = 0; c < matrix->getCols(); c++)
		{
			of << r << "\t" << c << "\t" << matrix->getAtPosition(r, c) << "\n";
		}
	}
	of.close();
	return geoHistogram;
}

// compute the Bhattacharrya distance between two images
double ShapeHistogram::bhattacharyyaDistance(Image refImage, Image sceneImage,
	AngleAccuracy angleAcc, int cols)
{
	ShapeHistogram refHistogram = shapeHistogram(refImage, angleAcc, cols);
	ShapeHistogram sceneHistogram = shapeHistogram(sceneImage, angleAcc, cols);
	return bhattacharyyaMetric(refHistogram, sceneHistogram);
}
