/*
 * Analysis.cpp
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

#include "imageModel/Matrix.h"
#include "imageModel/Point.h"
#include "imageModel/Line.h"
#include "imageModel/Edge.h"
#include "imageModel/Image.h"

#include "histograms/ShapeHistogram.h"
#include "pht/PHTEntry.h"
#include "pht/PHoughTransform.h"
#include "correlation/CrossCorrelation.h"

#include "pointInterest/Treatments.h"
#include "pointInterest/Segmentation.h"
#include "pointInterest/GeometricHistgoram.h"
#include "pointInterest/ProHoughTransform.h"
#include "pointInterest/LandmarkDetection.h"

#include "Analysis.h"

Analysis::Analysis()
{
	// TODO Auto-generated constructor stub

}

Analysis::~Analysis()
{
	// TODO Auto-generated destructor stub
}

Analysis::Analysis(ptr_Treatments t)
{
	treatment = t;
}
vector<ptr_Line> Analysis::segment(Image image, int minDistance)
{

	Segmentation sg;
	treatment = &sg;
	treatment->setRefImage(image);

	vector<ptr_Line> lines = sg.segment(minDistance);
	return lines;
}

ShapeHistogram Analysis::geomHistogram(Image image, AngleAccuracy angleAcc,
	int cols)
{
	GeometricHistgoram geoHistogram;
	treatment = &geoHistogram;
	treatment->setRefImage(image);
	return geoHistogram.geomHistogram(angleAcc, cols);
}
double Analysis::bhattacharyyaDistance(Image sceneImage, AngleAccuracy angleAcc,
	int cols)
{
	GeometricHistgoram geoHistogram;
	treatment = &geoHistogram;
	return geoHistogram.bhattacharyyaDistance(sceneImage, angleAcc, cols);
}
PHoughTransform Analysis::phtEntriesTable(Image image)
{
	ProHoughTransform pht;
	treatment = &pht;
	treatment->setRefImage(image);
	return pht.constructPHT();
}
vector<ptr_Point> Analysis::estimatedLandmarks(Image sceneImage,
	AngleAccuracy acc, int cols, int templSize, int sceneSize)
{
	LandmarkDetection lmd;
	lmd.setRefImage(treatment->getRefImage());
	treatment = &lmd;
	return lmd.landmarksAutoDectect(sceneImage,acc,cols,templSize,sceneSize);
}
