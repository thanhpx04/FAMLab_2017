/*
 * LandmarkDetection.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: linh
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

#include "../segmentation/Canny.h"
#include "../segmentation/Thresholds.h"

#include "../histograms/ShapeHistogram.h"
#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../pht/GHTInPoint.h"
#include "../correlation/CrossCorrelation.h"

#include "Treatments.h"
#include "Segmentation.h"
#include "GeometricHistgoram.h"
#include "ProHoughTransform.h"
#include "LandmarkDetection.h"

LandmarkDetection::LandmarkDetection()
{
	// TODO Auto-generated constructor stub

}

LandmarkDetection::~LandmarkDetection()
{
	// TODO Auto-generated destructor stub
}

vector<Point> LandmarkDetection::landmarksAutoDectect(Image sceneImage,
	AngleAccuracy acc, int cols, int templSize, int sceneSize, Point &ePoint,
	double &angleDiff)
{
	vector<Point> result;
	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.getListOfManualLandmarks();
	vector<Line> mLines = modelImage.getListOfLines();
	vector<Line> sLines = sceneImage.getListOfLines();
	int width = modelImage.getGrayMatrix()->getCols();
	int height = modelImage.getGrayMatrix()->getRows();

	ShapeHistogram mHistogram;
	vector<LocalHistogram> mLocalHist = mHistogram.constructPGH(mLines);
	mHistogram.constructPGHMatrix(mLocalHist, acc, cols);

	ShapeHistogram sHistogram;
	vector<LocalHistogram> sLocalHist = sHistogram.constructPGH(sLines);
	sHistogram.constructPGHMatrix(sLocalHist, acc, cols);

	double bhatt = bhattacharyyaMetric(mHistogram, sHistogram);
	cout << "\nBhattacharrya: " << bhatt << endl;
	//if (bhatt > 0.9)
	//{
	PHoughTransform mpht;
	mpht.setRefPoint(Point(width / 2, height / 2));

	vector<PHTEntry> entriesTable = mpht.constructPHTTable(mLines);
	vector<Point> phtEsLM = phtLandmarks(entriesTable, mpht.getRefPoint(), sLines,
		width, height, manualLMs, angleDiff, ePoint);

	cout << "\n Number of landmarks (pht): " << phtEsLM.size();
	cout << "\nAngle difference: " << angleDiff << endl;
	if (phtEsLM.size() > 0)
	{
		result = verifyLandmarks(modelImage, sceneImage, manualLMs, phtEsLM,
			templSize, sceneSize, angleDiff, ePoint);
		//result = phtEsLM;

	}
	entriesTable.clear();
	phtEsLM.clear();
	//}

	return result;
}

vector<Point> LandmarkDetection::landmarksAutoDectect2(Image &sceneImage,
	int templSize, int sceneSize)
{
	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.getListOfManualLandmarks();
	ProHoughTransform proHT;
	proHT.setRefImage(modelImage);
	double angle = 0;
	Point ePoint, mPoint;
	vector<Point> phtEsLM = proHT.generalTransform(sceneImage, angle, ePoint,
		mPoint);
	vector<Point> result;
	sceneImage.rotate(ePoint, angle, 1);

	if (phtEsLM.size() > 0)
	{
		cout << "\nAngle difference: " << angle << endl;
		result = verifyLandmarks2(modelImage, sceneImage, manualLMs, phtEsLM,
			templSize, sceneSize);
		//result = phtEsLM;
	}
	phtEsLM.clear();
	return result;
}

void LandmarkDetection::landmarksOnDir(string modelName, string folderScene,
	vector<string> sceneImages, AngleAccuracy acc, int cols, int templSize,
	int sceneSize, Point &ePoint, double &angleDiff, string saveFolder)
{

	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.getListOfManualLandmarks();
	vector<Line> mLines = modelImage.getListOfLines();
	int width = modelImage.getGrayMatrix()->getCols();
	int height = modelImage.getGrayMatrix()->getRows();

	ShapeHistogram mHistogram;
	vector<LocalHistogram> mLocalHist = mHistogram.constructPGH(mLines);
	mHistogram.constructPGHMatrix(mLocalHist, acc, cols);

	ShapeHistogram sHistogram;
	vector<LocalHistogram> sLocalHist;
	Image *sceneImage;
	for (size_t i = 0; i < sceneImages.size(); i++)
	{
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;
		vector<Point> result;
		angleDiff = 0;
		sceneImage = new Image(folderScene + "/" + sceneName);
		vector<Line> sLines = sceneImage->getListOfLines();

		sLocalHist = sHistogram.constructPGH(sLines);
		sHistogram.constructPGHMatrix(sLocalHist, acc, cols);

		double bhatt = bhattacharyyaMetric(mHistogram, sHistogram);
		cout << "\nBhattacharrya: " << bhatt << endl;
		PHoughTransform mpht;
		mpht.setRefPoint(Point(width / 2, height / 2));

		vector<PHTEntry> entriesTable = mpht.constructPHTTable(mLines);
		vector<Point> phtEsLM = phtLandmarks(entriesTable, mpht.getRefPoint(),
			sLines, width, height, manualLMs, angleDiff, ePoint);

		cout << "\n Number of landmarks (pht): " << phtEsLM.size();
		cout << "\nAngle difference: " << angleDiff << endl;

		if (phtEsLM.size() > 0)
		{
			result = verifyLandmarks(modelImage, *sceneImage, manualLMs, phtEsLM,
				templSize, sceneSize, angleDiff, ePoint);

		}
		entriesTable.clear();
		phtEsLM.clear();

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName + ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << result.size() << "\n";
		Point pk;
		for (size_t k = 0; k < result.size(); k++)
		{
			pk = result.at(k);
			inFile << pk.getX() << "\t" << pk.getY() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();
		delete sceneImage;
	}

}
void LandmarkDetection::landmarksOnDir2(string modelName, string folderScene,
	vector<string> sceneImages, string saveFolder)
{

	Image modelImage = Treatments::refImage;

	int rows = modelImage.getGrayMatrix()->getRows();
	int cols = modelImage.getGrayMatrix()->getCols();
	ptr_IntMatrix mMatrix = new Matrix<int>(rows, cols, 0);
	*mMatrix = *(Treatments::refImage.getGrayMatrix());
	int mThresholdValue = (int) Treatments::refImage.getThresholdValue();
	ptr_IntMatrix mbinMatrix = binaryThreshold(mMatrix, mThresholdValue, 255);
	ptr_IntMatrix mgradirection = new Matrix<int>(rows, cols, -1);
	ptr_IntMatrix mcannyMatrix = cannyProcess2(mbinMatrix, mThresholdValue,
		3 * mThresholdValue, mgradirection);
	vector<Point> manualLMs = modelImage.getListOfManualLandmarks();
	// Construct the R-table
	Point center(cols / 2, rows / 2);
	RTable rentries = rTableConstruct(mgradirection, center);

	for (size_t i = 191; i < sceneImages.size(); i++)
	{
		Image *sceneImage;
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;

		sceneImage = new Image(folderScene + "/" + sceneName);

		int sThresholdValue = sceneImage->getThresholdValue();
		ptr_IntMatrix sbinMatrix = new Matrix<int>(
			*(binaryThreshold(sceneImage->getGrayMatrix(), sThresholdValue, 255)));
		ptr_IntMatrix gradirection = new Matrix<int>(rows, cols, -1);
		ptr_IntMatrix cannyMatrix = new Matrix<int>(
			*(cannyProcess2(sbinMatrix, sThresholdValue, 3 * sThresholdValue,
				gradirection)));

		Point sPoint = houghSpace(gradirection, rentries);
		vector<Point> eslm = detectLandmarks(center, sPoint, manualLMs);

		// compute centroid of model, scene and centroid of model in scene
		Point mPoint, ePoint;
		Line sLine = principalAxis(gradirection, ePoint);
		Line mLine = principalAxis(mgradirection, mPoint);
		int drcx = center.getX() - mPoint.getX();
		int drcy = center.getY() - mPoint.getY();
		mPoint.setX(sPoint.getX() - drcx);
		mPoint.setY(sPoint.getY() - drcy);

		// compute angle
		int kx = mPoint.getX() - mLine.getEnd().getX();
		int ky = mPoint.getY() - mLine.getEnd().getY();
		Line mLine2(mPoint, Point(mPoint.getX() - kx, mPoint.getY() - ky));
		Point inPoint = sLine.intersection(mLine2);
		double anglek = sLine.angleLines(mLine);
		double angle = 0;

		if (anglek < 90)
		{ // keep rotation
			if (mPoint.getX() > ePoint.getX())
			{
				anglek = -anglek;
			}
		}
		else
		{ // inverse rotation
			if (mPoint.getX() < ePoint.getX())
			{
				anglek = -anglek;
			}
		}
		angle = -anglek;
		if (isnan(angle))
			angle = 0;
		int dx = mPoint.getX() - ePoint.getX();
		int dy = mPoint.getY() - ePoint.getY();
		Point pi;
		for (size_t i = 0; i < eslm.size(); i++)
		{
			pi = eslm.at(i);
			eslm.at(i).setX(pi.getX() - dx);
			eslm.at(i).setY(pi.getY() - dy);
		}
		vector<Point> estLandmarks;
		sceneImage->rotate(ePoint, angle, 1);

		if (eslm.size() > 0)
		{
			cout << "\nAngle difference: " << angle << endl;
			cout << "\n Number of landmarks (ght): " << eslm.size();
			estLandmarks = verifyLandmarks2(modelImage, *sceneImage, manualLMs, eslm,
				100, 300);
		}

		cout << "\n Number of landmarks (matching): " << estLandmarks.size();

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName + ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << estLandmarks.size() << "\n";
		Point pk;
		for (size_t k = 0; k < estLandmarks.size(); k++)
		{
			pk = estLandmarks.at(k);
			inFile << pk.getX() << " " << rows - pk.getY() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();

		estLandmarks.clear();
		eslm.clear();
		delete sbinMatrix;
		delete gradirection;
		delete cannyMatrix;
		delete sceneImage;
	}
	delete mMatrix;
	delete mgradirection;
	delete mcannyMatrix;
}
