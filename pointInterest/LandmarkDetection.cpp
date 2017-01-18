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
	vector<Point> manualLMs = modelImage.getListOfManualLandmarks();
	int rows = modelImage.getGrayMatrix()->getRows();
	int cols = modelImage.getGrayMatrix()->getCols();

	ptr_IntMatrix mbinMatrix = binaryThreshold(modelImage.getGrayMatrix(),
		(int) modelImage.getThresholdValue(), 255);
	ptr_IntMatrix mgradirection = new Matrix<int>(rows, cols, -1);
	ptr_IntMatrix mcannyMatrix = cannyProcess2(mbinMatrix,
		(int) modelImage.getThresholdValue(),
		3 * (int) modelImage.getThresholdValue(), mgradirection);
	Point center(cols / 2, rows / 2);
	RTable rentries = rTableConstruct(mgradirection, center);

	for (size_t i = 0; i < sceneImages.size(); i++)
	{
		Image *sceneImage;
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;

		sceneImage = new Image(folderScene + "/" + sceneName);

		ptr_IntMatrix sbinMatrix = binaryThreshold(sceneImage->getGrayMatrix(),
			(int) sceneImage->getThresholdValue(), 255);
		ptr_IntMatrix sgradirection = new Matrix<int>(rows, cols, -1);
		ptr_IntMatrix scannyMatrix = cannyProcess2(sbinMatrix,
			(int) sceneImage->getThresholdValue(),
			3 * (int) sceneImage->getThresholdValue(), sgradirection);

		Point ePoint = houghSpace(sgradirection, rentries);
		double angle = 0;
		vector<Point> eslm = detectLandmarks(center, ePoint,
			modelImage.getListOfManualLandmarks(), angle);

		cout << "\n Number of landmarks (pht): " << eslm.size();

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName + ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << eslm.size() << "\n";
		Point pk;
		for (size_t k = 0; k < eslm.size(); k++)
		{
			pk = eslm.at(k);
			inFile << pk.getX() << " " << rows - pk.getY() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();

		delete sbinMatrix;
		delete sgradirection;
		delete scannyMatrix;
		eslm.clear();
		delete sceneImage;
	}
	delete mbinMatrix;
	delete mgradirection;
	delete mcannyMatrix;
}
