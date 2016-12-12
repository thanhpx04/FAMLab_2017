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

#include "MAELab.h"

vector<ptr_Line> segment(ptr_Treatments treatment, Image image, int minDistance)
{

	Segmentation sg;
	treatment = &sg;
	treatment->setRefImage(image);

	vector<ptr_Line> lines = sg.segment(minDistance);
	return lines;
}

ShapeHistogram geomHistogram(ptr_Treatments treatment, Image image,
	AngleAccuracy angleAcc, int cols)
{
	GeometricHistgoram geoHistogram;
	treatment = &geoHistogram;
	treatment->setRefImage(image);
	return geoHistogram.geomHistogram(angleAcc, cols);
}
double bhattacharyyaDistance(ptr_Treatments treatment, Image sceneImage,
	AngleAccuracy angleAcc, int cols)
{
	GeometricHistgoram geoHistogram;
	treatment = &geoHistogram;
	return geoHistogram.bhattacharyyaDistance(sceneImage, angleAcc, cols);
}
PHoughTransform phtEntriesTable(ptr_Treatments treatment, Image image)
{
	ProHoughTransform pht;
	treatment = &pht;
	treatment->setRefImage(image);
	return pht.constructPHT();
}
vector<ptr_Point> estimatedLandmarks(ptr_Treatments treatment, Image sceneImage,
	AngleAccuracy acc, int cols, int templSize, int sceneSize)
{
	LandmarkDetection lmd;
	lmd.setRefImage(treatment->getRefImage());
	treatment = &lmd;
	return lmd.landmarksAutoDectect(sceneImage, acc, cols, templSize, sceneSize);
}
