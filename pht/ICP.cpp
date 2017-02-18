/*
 * ICP.cpp
 *
 *  Created on: Jan 31, 2017
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
#include "../utils/Drawing.h"
#include "GHTInPoint.h"
#include "../io/Reader.h"
#include "PCA.h"
#include "SVD.h"
#include "../correlation/CrossCorrelation.h"
#include "ICP.h"

/*Point centerPoint(vector<Point> listPoints)
 {
 int totalX = 0, totalY = 0;
 int size = listPoints.size();
 for (int i = 0; i < size; i++)
 {
 totalX += listPoints.at(i).getX();
 totalY += listPoints.at(i).getY();
 }
 return Point(totalX / size, totalY / size);
 }
 void icpmethod(vector<Point> modelPoints, vector<Point> scenePoints)
 {
 Point mCenter = centerPoint(modelPoints);
 Point sCenter = centerPoint(scenePoints);
 int dx = sCenter.getX() - mCenter.getX();
 int dy = sCenter.getY() - mCenter.getY();
 // translate scene to model
 for(size_t i = 0; i < modelPoints.size();i++)
 {
 int x = modelPoints.at(i).getX();
 int y = modelPoints.at(i).getY();
 modelPoints.at(i).setX(x+dx);
 modelPoints.at(i).setY(x+dy);
 }
 }*/
double errorSquared(vector<Line> pairPoints) {
	double result = 0;
	int size = pairPoints.size();
	for (int i = 0; i < size; i++) {
		Point begin = pairPoints.at(i).getBegin();
		Point end = pairPoints.at(i).getEnd();
		result += (pow((double) (end.getX() - begin.getX()), 2)
				+ pow((double) (end.getY() - begin.getY()), 2));
	}
	return sqrt(result / size);
}
Matrix<float> equivalent(Point center, vector<Point> listPoints) {
	Matrix<float> result(2, (int) listPoints.size(), 0);
	Point pi;
	float dx = 0.0, dy = 0.0;
	for (size_t i = 0; i < listPoints.size(); i++) {
		pi = listPoints.at(i);
		dx = pi.getX() - center.getX();
		dy = pi.getY() - center.getY();
		result.setAtPosition(0, (int) i, dx);
		result.setAtPosition(1, (int) i, dy);
	}
	return result;
}

void icpMethod(Image modelGray, Image &sceneGray) {
	Matrix<float> m(2, 2, 0.0);
	m.setAtPosition(0, 0, 5);
	m.setAtPosition(0, 1, 5);
	m.setAtPosition(1, 0, -1);
	m.setAtPosition(1, 1, 7);
	Matrix<float> v(2, 2, 0.0);
	Matrix<float> s(2, 2, 0.0);
	Matrix<float> u(2, 2, 0.0);
	//svdMethod(m, u, s, v);

	// ============================ print the result ============================
	/*cout << "\n Matrix m:" << endl;
	 for (int r = 0; r < m.getRows(); r++)
	 {
	 for (int c = 0; c < m.getCols(); c++)
	 {
	 cout << "\t" << m.getAtPosition(r, c);
	 }
	 cout << "\n";
	 }
	 cout << "\n Matrix mT:" << endl;
	 Matrix<float> mT = m.transposition(0.0);
	 for (int r = 0; r < mT.getRows(); r++)
	 {
	 for (int c = 0; c < mT.getCols(); c++)
	 {
	 cout << "\t" << mT.getAtPosition(r, c);
	 }
	 cout << "\n";
	 }
	 cout << "\n Matrix v:" << endl;
	 for (int r = 0; r < v.getRows(); r++)
	 {
	 for (int c = 0; c < v.getCols(); c++)
	 {
	 cout << "\t" << v.getAtPosition(r, c);
	 }
	 cout << "\n";
	 }
	 cout << "\n Matrix s:" << endl;
	 for (int r = 0; r < s.getRows(); r++)
	 {
	 for (int c = 0; c < s.getCols(); c++)
	 {
	 cout << "\t" << s.getAtPosition(r, c);
	 }
	 cout << "\n";
	 }
	 cout << "\n Matrix u:" << endl;
	 for (int r = 0; r < u.getRows(); r++)
	 {
	 for (int c = 0; c < u.getCols(); c++)
	 {
	 cout << "\t" << u.getAtPosition(r, c);
	 }
	 cout << "\n";
	 }*/
	// ==================================== end print the result =====================================
	// PCA
	int rows = modelGray.getGrayMatrix()->getRows();
	int cols = modelGray.getGrayMatrix()->getCols();
	vector<Point> modelPoints;
	ptr_IntMatrix modelGrandient = new Matrix<int>(rows, cols, -1);
	*modelGrandient = *(getGradientDMatrix(modelGray, modelPoints));
	vector<Point> modelPoints2(modelPoints);
	Point mPoint;
	Line mAxis = principalAxis(modelGrandient, mPoint);
	Point mend = mAxis.getEnd();

	vector<Point> scenePoints;
	ptr_IntMatrix sceneGrandient = new Matrix<int>(rows, cols, -1);
	*sceneGrandient = *(getGradientDMatrix(sceneGray, scenePoints));
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
	// translate the scene
	for (size_t i = 0; i < scenePoints.size(); i++) {
		pi = scenePoints.at(i);
		scenePoints.at(i).setX(pi.getX() + dx);
		scenePoints.at(i).setY(pi.getY() + dy);
	}
	double angleR = rotateDirection(mAxis, sAxis, angle);
	cout << "\nAngle difference: " << angleR << endl;
	// rotate the scene
	for (size_t i = 0; i < scenePoints.size(); i++) {
		pi = scenePoints.at(i);
		int xnew = 0, ynew = 0;
		rotateAPoint(pi.getX(), pi.getY(), mPoint, angleR, 1, xnew, ynew);
		scenePoints.at(i).setX(xnew);
		scenePoints.at(i).setY(ynew);
	}
	// hien thi thu ket qua ===========================================

	RGB color;
	color.R = 255;
	color.G = color.B = 0;
	for (size_t i = 0; i < scenePoints.size(); i++) {
		pi = scenePoints.at(i);
		if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
				&& pi.getY() < rows) {
			sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(),
					color);

		}
	}
	color.G = 255;

	for (size_t i = 0; i < modelPoints.size(); i++) {
		pi = modelPoints.at(i);
		if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
				&& pi.getY() < rows) {
			sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(),
					color);

		}
	}

	// ============================== ket thuc hien thi ket qua ==============================
	// finding closest points
	//vector<Line> pairPoints;
	double minerror = DBL_MAX;
	double t;
	//do
	//{
	vector<Point> modelCorr, sceneCorr;
	for (size_t i = 0; i < modelPoints.size(); i++) {
		Point pm = modelPoints.at(i);
		double minDist = DBL_MAX;
		Point pk;
		for (size_t j = 0; j < scenePoints.size(); j++) {
			Point ps = scenePoints.at(j);
			Line temp(pm, ps);
			if (temp.getLength() < minDist) {
				minDist = temp.getLength();
				pk = ps;
			}
		}
		modelCorr.push_back(pm);
		sceneCorr.push_back(pk);
		//pairPoints.push_back(Line(pm, pk));
		//drawingLine(*sceneGray.getRGBMatrix(), Line(pm, pk), color);
	}
	if (modelCorr.size() == sceneCorr.size()) {
		Point mi, si;
		Matrix<float> P(2, (int) sceneCorr.size(), 0.0), Q(2,
				(int) modelCorr.size(), 0.0);
		for (size_t i = 0; i < modelCorr.size(); i++) {
			mi = modelCorr.at(i);
			si = sceneCorr.at(i);
			int mx = mi.getX() - mPoint.getX();
			int my = mi.getY() - mPoint.getY();
			Q.setAtPosition(0, (int) i, mx);
			Q.setAtPosition(1, (int) i, my);
			int sx = si.getX() - mPoint.getX();
			int sy = si.getY() - mPoint.getY();
			P.setAtPosition(0, (int) i, sx);
			P.setAtPosition(1, (int) i, sy);
		}
		Matrix<float> M(2, 2, 0.0);
		M = P.multiply(Q.transposition(0.0), 0.0);
		for (int r = 0; r < M.getRows(); r++) {
			for (int c = 0; c < M.getCols(); c++) {
				cout << "\t" << M.getAtPosition(r, c);
			}
			cout << "\n";
		}
		//M.setAtPosition(0,0,9.969);
		//M.setAtPosition(0,1,-4.2965);
		//M.setAtPosition(1,0,-3.6838);
		//	M.setAtPosition(1,1,22.2107);
		svdMethod(M, u, s, v);

		// ============================ print the result ============================
		/*cout << "\n Matrix m:" << endl;
		 for (int r = 0; r < m.getRows(); r++)
		 {
		 for (int c = 0; c < m.getCols(); c++)
		 {
		 cout << "\t" << m.getAtPosition(r, c);
		 }
		 cout << "\n";
		 }*/
		cout << "\n Matrix mT:" << endl;
		Matrix<float> mT = M.transposition(0.0);
		for (int r = 0; r < mT.getRows(); r++) {
			for (int c = 0; c < mT.getCols(); c++) {
				cout << "\t" << mT.getAtPosition(r, c);
			}
			cout << "\n";
		}
		cout << "\n Matrix v:" << endl;
		for (int r = 0; r < v.getRows(); r++) {
			for (int c = 0; c < v.getCols(); c++) {
				cout << "\t" << v.getAtPosition(r, c);
			}
			cout << "\n";
		}
		cout << "\n Matrix s:" << endl;
		for (int r = 0; r < s.getRows(); r++) {
			for (int c = 0; c < s.getCols(); c++) {
				cout << "\t" << s.getAtPosition(r, c);
			}
			cout << "\n";
		}
		cout << "\n Matrix u:" << endl;
		for (int r = 0; r < u.getRows(); r++) {
			for (int c = 0; c < u.getCols(); c++) {
				cout << "\t" << u.getAtPosition(r, c);
			}
			cout << "\n";
		}
		Matrix<float> rm = v.multiply(u.transposition(0.0), 0.0);
		cout << "\n Matrix r:" << endl;
		for (int r = 0; r < rm.getRows(); r++) {
			for (int c = 0; c < rm.getCols(); c++) {
				cout << "\t" << rm.getAtPosition(r, c);
			}
			cout << "\n";
		}
		// rotate the scene and display
		color.R = color.G = color.B = 0;
		color.G = 255;
		Point pi2;
		float r1 = rm.getAtPosition(0, 0);
		float r2 = rm.getAtPosition(0, 1);
		float r3 = rm.getAtPosition(1, 0);
		float r4 = rm.getAtPosition(1, 1);
		for (size_t i = 0; i < modelPoints.size(); i++) {
			pi2 = modelPoints.at(i);
			int x = pi2.getX(), y = pi2.getY();
			pi2.setX(r1 * x + r2 * y);
			pi2.setX(r3 * x + r4 * y);
			if (pi2.getX() >= 0 && pi2.getX() < cols && pi2.getY() >= 0
					&& pi2.getY() < rows) {
				sceneGray.getRGBMatrix()->setAtPosition(pi2.getY(), pi2.getX(),
						color);
			}
		}
		// ==================================== end print the result =====================================
	}
}
struct comparey {
	bool operator()(Point p1, Point p2) {
		return p1.getY() < p2.getY();
	}
} yComparation;
vector<Point> icpMethod2(Image modelGray, Image &sceneGray,
		vector<Point> mnLandmarks) {
	// PCA
	int rows = modelGray.getGrayMatrix()->getRows();
	int cols = modelGray.getGrayMatrix()->getCols();
	ptr_IntMatrix lastModel = new Matrix<int>(rows, cols, 0);
	ptr_IntMatrix lastScene = new Matrix<int>(rows, cols, 0);
	vector<Point> modelPoints;
	ptr_IntMatrix modelGrandient = new Matrix<int>(rows, cols, -1);
	*modelGrandient = *(getGradientDMatrix(modelGray, modelPoints));

	Point mPoint;
	Line mAxis = principalAxis(modelGrandient, mPoint);
	Point mend = mAxis.getEnd();

	vector<Point> scenePoints;
	ptr_IntMatrix sceneGrandient = new Matrix<int>(rows, cols, -1);
	*sceneGrandient = *(getGradientDMatrix(sceneGray, scenePoints));
	vector<Point> scenePoints2(scenePoints);
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
	// translate the scene
	double firstAngle = angle;
	double angleR = rotateDirection(mAxis, sAxis, angle);
	cout << "\nAngle difference: " << angleR << endl;
	// rotate and translate the scene
	for (size_t i = 0; i < scenePoints.size(); i++) {
		pi = scenePoints.at(i);
		int xnew = 0, ynew = 0;
		rotateAPoint(pi.getX() + dx, pi.getY() + dy, mPoint, angleR, 1, xnew,
				ynew);
		scenePoints.at(i).setX(xnew);
		scenePoints.at(i).setY(ynew);
	}

	RGB color;
	color.R = 255;
	color.G = color.B = 0;

	// hien thu ket qua lan thu nhat
	// drawing scene image
	/*for (size_t i = 0; i < scenePoints.size(); i++) {
	 pi = scenePoints.at(i);
	 if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
	 && pi.getY() < rows) {
	 sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(),
	 color);

	 }
	 }*/
	// drawing model image
	color.G = 255;
	for (size_t i = 0; i < modelPoints.size(); i++) {
		pi = modelPoints.at(i);
		if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
				&& pi.getY() < rows) {
			sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(),
					color);
			lastModel->setAtPosition(pi.getY(), pi.getX(), 255);
		}
	}
	size_t nlimit = modelPoints.size() / 2;
	std::sort(modelPoints.begin(), modelPoints.end(), yComparation);
	vector<Point> newModel(modelPoints.begin(), modelPoints.begin() + nlimit);

	vector<Point> sceneTemp(scenePoints);
	// hien thi thu ket qua ===========================================
	int index = 0;
	double minAngle = 360;
	while (angle > 1.5) {
		index++;
		//double angle1 = angle;
		std::sort(sceneTemp.begin(), sceneTemp.end(), yComparation);
		vector<Point> newScene(sceneTemp.begin(), sceneTemp.begin() + nlimit);
		Point cPoint1, cPoint2;
		Line l1 = principalAxis(newModel, cPoint1);
		Line l2 = principalAxis(newScene, cPoint2);
		angle = l1.angleLines(l2);
		cout << "\nAngle difference 2: " << angle << endl;
		if (index > 20 || angle < 1.5) {
			if (angle < minAngle)
				minAngle = angle;
		}
		Point diff = cPoint1 - cPoint2;
		l2.setBegin(l1.getBegin());
		l2.setEnd(
				Point(l2.getEnd().getX() + diff.getX(),
						l2.getEnd().getY() + diff.getY()));
		double angleR2 = rotateDirection(l1, l2, l1.angleLines(l2));

		//cout << "\nAngle difference 22: " << angleR2 << endl;
		Point psn;

		for (size_t i = 0; i < sceneTemp.size(); i++) {
			psn = sceneTemp.at(i);
			int xnew = psn.getX() + diff.getX(), ynew = psn.getY()
					+ diff.getY();
			rotateAPoint(xnew, ynew, mPoint, angleR2, 1, xnew, ynew);
			sceneTemp.at(i).setX(xnew);
			sceneTemp.at(i).setY(ynew);
		}
		if (index > 30) {
			if ((int) angle == (int) minAngle) {
				break;
			}
		}
	}
	cout << "\nMin angle: " << minAngle << endl;
	if (minAngle > 10) {
		sceneTemp = scenePoints;
	}
	color.G = 255;
	color.B = 255;
	color.R = 0;
	for (size_t i = 0; i < sceneTemp.size(); i++) {
		pi = sceneTemp.at(i);
		if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
				&& pi.getY() < rows) {
			sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(),
					color);
			lastScene->setAtPosition(pi.getY(), pi.getX(), 255);
		}
	}

	vector<Point> result;
	if (mnLandmarks.size() > 0) {
		result = verifyLandmarks2(lastModel, lastScene, mnLandmarks,
				mnLandmarks, 100, 300);
	}
	/*for (size_t i = 0; i < result.size(); i++) {
	 pi = result.at(i);
	 fillCircle(*sceneGray.getRGBMatrix(), pi, 5, color);
	 }*/
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
	color.R = 255;
	color.G = color.B = 0;
	for (size_t i = 0; i < result.size(); i++) {
		pi = result.at(i);
		int xnew = pi.getX() + mback.getX(), ynew = pi.getY() + mback.getY();
		rotateAPoint(xnew, ynew, lsPoint1, mangle, 1, xnew, ynew);
		pi.setX(xnew);
		pi.setY(ynew);
		result.at(i).setX(xnew);
		result.at(i).setY(ynew);
		fillCircle(*sceneGray.getRGBMatrix(), pi, 5, color);
	}

	delete modelGrandient;
	delete sceneGrandient;
	return result;
	// ============================== ket thuc hien thi ket qua ==============================
}

void icpFolder(string folderScene, vector<string> sceneImages, Image model,
		vector<Point> mnLandmarks) {
	int rows = model.getRGBMatrix()->getRows();
	int cols = model.getRGBMatrix()->getCols();
	vector<Point> result;
	for (size_t i = 100; i < 120; i++) {
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;
		vector<Point> result;
		Image sceneImage(folderScene + "/" + sceneName);
		result = icpMethod2(model, sceneImage, mnLandmarks);

		// save TPS
		string path = "/home/linh/Desktop/results/2017/mg/17fevrier/"
				+ sceneName;
		ofstream inFile((path + ".TPS").c_str());
		inFile << "LM=" << result.size() << "\n";
		Point epk;
		RGB color;
		color.R = color.G = 255;
		color.B = 0;
		for (size_t k = 0; k < result.size(); k++) {
			epk = result.at(k);
			inFile << epk.getX() << " " << rows - epk.getY() << "\n";
		}
		inFile << "IMAGE=" << path << "\n";
		inFile.close();

		saveRGB(path.c_str(), sceneImage.getRGBMatrix());

		result.clear();
	}
}

/*void icpmethod(Image modelGray, Image &sceneGray)
 {
 int rows = modelGray.getGrayMatrix()->getRows();
 int cols = modelGray.getGrayMatrix()->getCols();
 vector<Point> modelPoints;
 ptr_IntMatrix modelGrandient = new Matrix<int>(rows, cols, -1);
 *modelGrandient = *(getGradientDMatrix(modelGray, modelPoints));
 vector<Point> modelPoints2(modelPoints);
 Point mPoint;
 Line mAxis = principalAxis(modelGrandient, mPoint);
 Point mend = mAxis.getEnd();

 vector<Point> scenePoints;
 ptr_IntMatrix sceneGrandient = new Matrix<int>(rows, cols, -1);
 *sceneGrandient = *(getGradientDMatrix(sceneGray, scenePoints));
 Point sPoint;
 Line sAxis = principalAxis(sceneGrandient, sPoint);

 double angle = mAxis.angleLines(sAxis); // in degree
 int dx = sPoint.getX() - mPoint.getX();
 int dy = sPoint.getY() - mPoint.getY();
 // translate scene to model
 for (size_t i = 0; i < modelPoints.size(); i++)
 {
 int x = modelPoints.at(i).getX();
 int y = modelPoints.at(i).getY();
 modelPoints.at(i).setX(x + dx);
 modelPoints.at(i).setY(y + dy);
 }
 // translate the endpoint of mAxis
 mend.setX(mend.getX() + dx);
 mend.setY(mend.getY() + dy);
 int xnew = 0, ynew = 0;
 rotateAPoint(mend.getX(), mend.getY(), sPoint, angle, 1, xnew, ynew);
 double distance1 = sAxis.perpendicularDistance(Point(xnew, ynew));
 xnew = ynew = 0;
 rotateAPoint(mend.getX(), mend.getY(), sPoint, -angle, 1, xnew, ynew);
 double distance2 = sAxis.perpendicularDistance(Point(xnew, ynew));
 if (distance2 < distance1)
 angle = -angle;
 cout << "\nAngle difference: " << angle << endl;
 for (size_t i = 0; i < modelPoints.size(); i++)
 {
 int x = modelPoints.at(i).getX();
 int y = modelPoints.at(i).getY();
 int xnew = 0, ynew = 0;
 rotateAPoint(x, y, sPoint, angle, 1, xnew, ynew);
 modelPoints.at(i).setX(xnew);
 modelPoints.at(i).setY(ynew);
 }
 RGB color;
 color.R = 255;
 color.G = color.B = 0;
 for (size_t i = 0; i < scenePoints.size(); i++)
 {
 Point pi = scenePoints.at(i);
 if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
 && pi.getY() < rows)
 {
 sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(), color);
 }
 }
 color.R = 0;
 color.B = 255;
 for (size_t i = 0; i < modelPoints.size(); i++)
 {
 Point pi = modelPoints.at(i);
 if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
 && pi.getY() < rows)
 {
 sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(), color);
 }
 }
 color.R = 255;
 fillCircle(*sceneGray.getRGBMatrix(), sPoint, 5, color);
 // finding closest points
 vector<Line> pairPoints;
 double minerror = DBL_MAX;
 double t;
 //do
 //{
 for (size_t i = 0; i < modelPoints.size(); i++)
 {
 Point pm = modelPoints.at(i);
 double minDist = DBL_MAX;
 Point pk;
 for (size_t j = 0; j < scenePoints.size(); j++)
 {
 Point ps = scenePoints.at(j);
 Line temp(pm, ps);
 if (temp.getLength() < minDist)
 {
 minDist = temp.getLength();
 pk = ps;
 }
 }
 pairPoints.push_back(Line(pm, pk));
 //drawingLine(*sceneGray.getRGBMatrix(), Line(pm, pk), color);
 }
 vector<Point> modelCorr, sceneCorr;
 Line l;
 for (size_t i = 0; i < pairPoints.size(); i++)
 {
 l = pairPoints.at(i);
 modelCorr.push_back(l.getBegin());
 sceneCorr.push_back(l.getEnd());
 }

 // apply svd
 Matrix<float> modelP = equivalent(sPoint, modelCorr);
 cout << "\nModel P: " << modelP.getRows() << "\t" << modelP.getCols();
 Matrix<float> sceneQ = equivalent(sPoint, sceneCorr);
 cout << "\nScene Q: " << sceneQ.getRows() << "\t" << sceneQ.getCols() << endl;
 Matrix<float> m = modelP.multiply(sceneQ.transposition(0), 0.0);
 Matrix<float> mT = m.transposition(0.0);
 Matrix<float> mTm = mT.multiply(m, 0);

 float mTm00 = mTm.getAtPosition(0, 0);
 float mTm01 = mTm.getAtPosition(0, 1);
 float mTm10 = mTm.getAtPosition(1, 0);
 float mTm11 = mTm.getAtPosition(1, 1);
 // pt bac 2
 float a = 1;
 float b = -(mTm00 + mTm11);
 float c = (mTm00 * mTm11) - (mTm01 * mTm10);
 vector<float> n0 = ptB2(a, b, c);
 Matrix<float> v(2, 2);
 Matrix<float> w(2, 2, 0.0);
 if (n0.size() != 0)
 {
 for (int i = 0; i < n0.size(); ++i)
 {
 float k = n0.at(i);
 w.setAtPosition(i, i, sqrt(k));
 float k1 = mTm00 - k;
 float k2 = -mTm01 / k1;
 float summ = (k2 * k2) + 1;
 v.setAtPosition(0, i, k2 / summ);
 v.setAtPosition(1, i, 1 / summ);
 }
 }
 cout << "\nV matrix: " << endl;
 for (int r = 0; r < 2; ++r)
 {
 for (int c = 0; c < 2; ++c)
 {
 cout << "\t" << v.getAtPosition(r, c);
 }
 cout << "\n";
 }

 Matrix<float> cv = mTm.multiply(v, 0.0);
 float sumc1 = 0, sumc2 = 0;

 sumc1 = cv.getAtPosition(0, 0) + cv.getAtPosition(1, 0);
 sumc2 = cv.getAtPosition(0, 1) + cv.getAtPosition(1, 1);
 cout << "\nU matrix: " << endl;
 Matrix<float> u(2, 2, 0.0);
 for (int r = 0; r < 2; ++r)
 {
 for (int c = 0; c < 2; ++c)
 {
 float rc = 0.0;
 if (c % 2 == 0)
 rc = cv.getAtPosition(r, c) / sumc1;
 else
 rc = cv.getAtPosition(r, c) / sumc2;
 cout << "\t" << rc;
 u.setAtPosition(r, c, rc);
 }
 cout << "\n";
 }
 Matrix<float> r = v.multiply(u.transposition(0.0), 0.0);
 for (size_t i = 0; i < modelPoints2.size(); i++)
 {
 int x = modelPoints2.at(i).getX();
 int y = modelPoints2.at(i).getY();
 int xnew = 0, ynew = 0;
 xnew = mPoint.getX()
 + (r.getAtPosition(0, 0) * (x - sPoint.getX())
 + r.getAtPosition(0, 1) * (y - sPoint.getY()));
 ynew = mPoint.getY()
 + (r.getAtPosition(1, 0) * (x - sPoint.getX())
 + r.getAtPosition(1, 1) * (y - sPoint.getY()));
 modelPoints2.at(i).setX(xnew);
 modelPoints2.at(i).setY(ynew);
 }
 for (size_t i = 0; i < modelPoints2.size(); i++)
 {
 Point pi = modelPoints2.at(i);
 if (pi.getX() >= 0 && pi.getX() < cols && pi.getY() >= 0
 && pi.getY() < rows)
 {
 sceneGray.getRGBMatrix()->setAtPosition(pi.getY(), pi.getX(), color);
 }
 }

 // compute stop criteria
 double err = errorSquared(pairPoints);
 t = minerror - err;
 minerror = err;

 //}
 //while (t > 1);
 }*/
