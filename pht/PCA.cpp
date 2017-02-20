/*
 * PCA.cpp
 *
 *  Created on: Feb 7, 2017
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <algorithm>
using namespace std;
#include "../utils/Converter.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"
#include "../correlation/CrossCorrelation.h"
#include "../utils/Drawing.h"
#include "../io/Reader.h"
#include "GHTInPoint.h"
#include "PCA.h"
double avgDistance(vector<Point> listPoints, Line axis)
{
	double totalDist = 0;
	size_t nPoints = listPoints.size();
	for (size_t j = 0; j < nPoints; j++)
	{
		Point pj = listPoints.at(j);
		double distance = axis.perpendicularDistance(pj);
		totalDist += distance;
	}
	return totalDist / (int) nPoints;
}

// compute centroid point from the gradient matrix of image
// output: centroid point and list points of edge.
Point centroidPoint(ptr_IntMatrix gradMatrix, vector<Point> &listPoints)
{
	int rows = gradMatrix->getRows();
	int cols = gradMatrix->getCols();
	int totalX = 0, totalY = 0, count = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (gradMatrix->getAtPosition(r, c) != -1)
			{
				listPoints.push_back(Point(c, r));
				totalX += c;
				totalY += r;
				count++;
			}
		}
	}
	return Point(totalX / count, totalY / count);
}

Line principalAxis(vector<Point> listOfPoints, Point &cPoint)
{
	double minAvgDist = DBL_MAX;
	Point sPoint;
	size_t nPoints = listOfPoints.size();
	int totalX = 0, totalY = 0;
	Point pi;
	for (size_t i = 0; i < nPoints; i++)
	{
		pi = listOfPoints.at(i);
		totalX += pi.getX();
		totalY += pi.getY();
	}
	cPoint.setX(totalX / (int) nPoints);
	cPoint.setY(totalY / (int) nPoints);
	pi.setX(0);
	pi.setY(0);
	for (size_t i = 0; i < nPoints; i++)
	{
		pi = listOfPoints.at(i);
		Line l(cPoint, pi);
		double avgDist = avgDistance(listOfPoints, l);
		if (avgDist < minAvgDist)
		{
			minAvgDist = avgDist;
			sPoint.setX(pi.getX());
			sPoint.setY(pi.getY());
		}
	}

	return Line(cPoint, sPoint);
}
Line principalAxis(ptr_IntMatrix gradMatrix, Point &cPoint)
{
	vector<Point> listOfPoints;
	cPoint = centroidPoint(gradMatrix, listOfPoints);
	return principalAxis(listOfPoints, cPoint);

}
// negative for clockwise, positive for counterclockwise
// two lines have the same origin
double rotateDirection(Line refLine, Line objLine, double angle)
{
	int xnew = 0, ynew = 0;
	Point origin = refLine.getBegin();
	Point rpoint = objLine.getEnd();
	rotateAPoint(rpoint.getX(), rpoint.getY(), origin, angle, 1, xnew, ynew);
	double distance1 = refLine.perpendicularDistance(Point(xnew, ynew));
	xnew = ynew = 0;
	rotateAPoint(rpoint.getX(), rpoint.getY(), origin, -angle, 1, xnew, ynew);
	double distance2 = refLine.perpendicularDistance(Point(xnew, ynew));
	if (distance2 < distance1)
		angle = -angle;
	return angle;
}
// estimated by using PCAI
struct comparey
{
	bool operator()(Point p1, Point p2)
	{
		return p1.getY() < p2.getY();
	}
} yComparation;

vector<Point> PCAI(vector<Point> modelPoints, Image sceneGray,
	vector<Point> mnLandmarks)
{
	int rows = sceneGray.getGrayMatrix()->getRows();
	int cols = sceneGray.getGrayMatrix()->getCols();
	ptr_IntMatrix lastModel = new Matrix<int>(rows, cols, 0);
	ptr_IntMatrix lastScene = new Matrix<int>(rows, cols, 0);

	Point mPoint;
	Line mAxis = principalAxis(modelPoints, mPoint);

	vector<Point> scenePoints;
	ptr_IntMatrix sceneGrandient = new Matrix<int>(rows, cols, -1);
	*sceneGrandient = *(getGradientDMatrix(sceneGray, scenePoints));
	vector<Point> scenePoints2(scenePoints); // keep the original points of the scene
	Point sPoint;
	Line sAxis = principalAxis(sceneGrandient, sPoint);
	Point send = sAxis.getEnd();
	double angle = mAxis.angleLines(sAxis);

	// difference between two centroids
	int dx = mPoint.getX() - sPoint.getX();
	int dy = mPoint.getY() - sPoint.getY();

	// move the scene to the model
	sAxis.setBegin(mPoint);
	send.setX(send.getX() + dx);
	send.setY(send.getY() + dy);
	sAxis.setEnd(send);
	Point pi;
	// Detecting the rotated direction
	double angleR = rotateDirection(mAxis, sAxis, angle);

	// rotate and translate the scene to match with the model
	for (size_t i = 0; i < scenePoints.size(); i++)
	{
		pi = scenePoints.at(i);
		int xnew = 0, ynew = 0;
		rotateAPoint(pi.getX() + dx, pi.getY() + dy, mPoint, angleR, 1, xnew, ynew);
		scenePoints.at(i).setX(xnew);
		scenePoints.at(i).setY(ynew);
	}

	// hien thu ket qua lan thu nhat
	for (size_t i = 0; i < modelPoints.size(); i++)
	{
		pi = modelPoints.at(i);
		if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
			&& pi.getY() < rows)
		{
			lastModel->setAtPosition(pi.getY(), pi.getX(), 255);
		}
	}

	size_t nlimit = modelPoints.size() / 2;
	std::sort(modelPoints.begin(), modelPoints.end(), yComparation);
	vector<Point> newModel(modelPoints.begin(), modelPoints.begin() + nlimit);
	vector<Point> sceneTemp(scenePoints); // copy the points of scene
	int index = 0;
	double minAngle = 360;
	while (angle > 1.5)
	{
		index++;
		std::sort(sceneTemp.begin(), sceneTemp.end(), yComparation);
		vector<Point> newScene(sceneTemp.begin(), sceneTemp.begin() + nlimit);
		Point cPoint1, cPoint2;
		Line l1 = principalAxis(newModel, cPoint1);
		Line l2 = principalAxis(newScene, cPoint2);
		angle = l1.angleLines(l2);
		if (index > 20 || angle < 1.5)
		{
			if (angle < minAngle)
				minAngle = angle;
		}
		Point diff = cPoint1 - cPoint2;
		l2.setBegin(l1.getBegin());
		l2.setEnd(
			Point(l2.getEnd().getX() + diff.getX(),
				l2.getEnd().getY() + diff.getY()));
		double angleR2 = rotateDirection(l1, l2, l1.angleLines(l2));

		Point psn;
		for (size_t i = 0; i < sceneTemp.size(); i++)
		{
			psn = sceneTemp.at(i);
			int xnew = psn.getX() + diff.getX(), ynew = psn.getY() + diff.getY();
			rotateAPoint(xnew, ynew, mPoint, angleR2, 1, xnew, ynew);
			sceneTemp.at(i).setX(xnew);
			sceneTemp.at(i).setY(ynew);
		}
		if (index > 30)
		{
			if ((int) angle == (int) minAngle)
			{
				break;
			}
		}
	}
	cout << "\nMin angle: " << minAngle << endl;
	if (minAngle > 10)
	{
		sceneTemp = scenePoints;
	}
	// display scene points
	for (size_t i = 0; i < sceneTemp.size(); i++)
	{
		pi = sceneTemp.at(i);
		if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
			&& pi.getY() < rows)
		{
			lastScene->setAtPosition(pi.getY(), pi.getX(), 255);
		}
	}

	vector<Point> result;
	if (mnLandmarks.size() > 0)
	{
		result = verifyLandmarks2(lastModel, lastScene, mnLandmarks, mnLandmarks,
			100, 300);
	}

	// compute the difference between original scene and rotated scene
	// to move-back the landmarks
	Point lsPoint1, lsPoint2;
	Line sl1 = principalAxis(scenePoints2, lsPoint1);
	Line sl2 = principalAxis(sceneTemp, lsPoint2);
	Point mback = lsPoint1 - lsPoint2;
	double mangle = sl1.angleLines(sl2);
	sl2.setBegin(sl1.getBegin());
	sl2.setEnd(
		Point(sl2.getEnd().getX() + mback.getX(),
			sl2.getEnd().getY() + mback.getY()));
	mangle = rotateDirection(sl1, sl2, mangle);
	for (size_t i = 0; i < result.size(); i++)
	{
		pi = result.at(i);
		int xnew = pi.getX() + mback.getX(), ynew = pi.getY() + mback.getY();
		rotateAPoint(xnew, ynew, lsPoint1, mangle, 1, xnew, ynew);
		pi.setX(xnew);
		pi.setY(ynew);
		result.at(i).setX(xnew);
		result.at(i).setY(ynew);
	}

	delete sceneGrandient;
	return result;
}
vector<Point> PCAI(Image modelGray, Image &sceneGray, vector<Point> mnLandmarks)
{
	// PCA
	int rows = modelGray.getGrayMatrix()->getRows();
	int cols = modelGray.getGrayMatrix()->getCols();
	//ptr_IntMatrix lastModel = new Matrix<int>(rows, cols, 0);
	///ptr_IntMatrix lastScene = new Matrix<int>(rows, cols, 0);
	vector<Point> modelPoints;
	ptr_IntMatrix modelGrandient = new Matrix<int>(rows, cols, -1);
	*modelGrandient = *(getGradientDMatrix(modelGray, modelPoints));

	vector<Point> scenePoints;
	ptr_IntMatrix sceneGrandient = new Matrix<int>(rows, cols, -1);
	*sceneGrandient = *(getGradientDMatrix(sceneGray, scenePoints));
	vector<Point> result = PCAI(modelPoints, sceneGray, mnLandmarks);

	delete modelGrandient;
	delete sceneGrandient;
	return result;
}
void pcaiFolder(string folderScene, vector<string> sceneImages, Image model,
	vector<Point> mnLandmarks,string savePath)
{
	int rows = model.getRGBMatrix()->getRows();
	int cols = model.getRGBMatrix()->getCols();
	vector<Point> modelPoints;
	ptr_IntMatrix modelGrandient = new Matrix<int>(rows, cols, -1);
	*modelGrandient = *(getGradientDMatrix(model, modelPoints));
	vector<Point> result;
	for (size_t i = 0; i < 20; i++)
	{
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;
		vector<Point> result;
		Image sceneImage(folderScene + "/" + sceneName);
		result = PCAI(modelPoints, sceneImage, mnLandmarks);

		// save TPS
		string path = savePath +"/" + sceneImage.getName();
		ofstream inFile((path + ".TPS").c_str());
		inFile << "LM=" << result.size() << "\n";
		Point epk;
		RGB color;
		color.R = color.G = 255;
		color.B = 0;
		for (size_t k = 0; k < result.size(); k++)
		{
			epk = result.at(k);
			inFile << epk.getX() << " " << rows - epk.getY() << "\n";
		}
		inFile << "IMAGE=" << path << "\n";
		inFile.close();

		result.clear();
	}
}
