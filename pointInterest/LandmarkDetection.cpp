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

#include "../histograms/ShapeHistogram.h"
#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../correlation/CrossCorrelation.h"

#include "Treatments.h"
#include "Segmentation.h"
#include "GeometricHistgoram.h"
#include "ProHoughTransform.h"
#include "LandmarkDetection.h"

LandmarkDetection::LandmarkDetection() {
	// TODO Auto-generated constructor stub

}

LandmarkDetection::~LandmarkDetection() {
	// TODO Auto-generated destructor stub
}

vector<Point> LandmarkDetection::landmarksAutoDectect(Image sceneImage,
		AngleAccuracy acc, int cols, int templSize, int sceneSize,
		Point &ePoint, double &angleDiff) {
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
	vector<Point> phtEsLM = phtLandmarks(entriesTable, mpht.getRefPoint(),
			sLines, width, height, manualLMs, angleDiff, ePoint);

	cout << "\n Number of landmarks (pht): " << phtEsLM.size();
	cout << "\nAngle difference: " << angleDiff << endl;
	if (phtEsLM.size() > 0) {
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
		int sceneSize, Point &ePoint, double &angleDiff, string saveFolder) {

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
	for (size_t i = 0; i < sceneImages.size(); i++) {
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;
		vector<Point> result;
		sceneImage = new Image(folderScene + "/" + sceneName);
		vector<Line> sLines = sceneImage->getListOfLines();

		sLocalHist = sHistogram.constructPGH(sLines);
		sHistogram.constructPGHMatrix(sLocalHist, acc, cols);
		cout << "\nLocal histogram" << endl;
		double bhatt = bhattacharyyaMetric(mHistogram, sHistogram);
		cout << "\nBhattacharrya: " << bhatt << endl;
		PHoughTransform mpht;
		mpht.setRefPoint(Point(width / 2, height / 2));

		vector<PHTEntry> entriesTable = mpht.constructPHTTable(mLines);
		vector<Point> phtEsLM = phtLandmarks(entriesTable, mpht.getRefPoint(),
				sLines, width, height, manualLMs, angleDiff, ePoint);

		cout << "\n Number of landmarks (pht): " << phtEsLM.size();
		cout << "\nAngle difference: " << angleDiff << endl;

		if (phtEsLM.size() > 0) {
			result = verifyLandmarks(modelImage, *sceneImage, manualLMs,
					phtEsLM, templSize, sceneSize, angleDiff, ePoint);

		}
		entriesTable.clear();
		phtEsLM.clear();

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName
				+ ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << result.size() << "\n";
		Point pk;
		for (size_t k = 0; k < result.size(); k++) {
			pk = result.at(k);
			inFile << pk.getX() << "\t" << pk.getY() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();
		delete sceneImage;
	}

}
