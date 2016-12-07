/*
 *
 *
 * Test file
 */
#include <iostream>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "imageModel/Point.h"
#include "imageModel/Line.h"
#include "imageModel/Edge.h"
#include "imageModel/Matrix.h"
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

int main()
{
	//cout.precision(16);
	cout << "MAELab test" << endl << endl;

	//Image image("/home/linh/Desktop/Temps/md/images/Md 009.JPG");
	Image modelImage("data/Md039.JPG");
	modelImage.readManualLandmarks("/home/linh/Desktop/Temps/md/landmarks/Md 039.TPS");

	Image sceneimage("data/Md_046.jpg");

	LandmarkDetection lm;
	ptr_Treatments tr = new LandmarkDetection();
	tr->setRefImage(modelImage);

	Analysis analys(tr);
	vector<ptr_Point> esLandmarks = analys.estimatedLandmarks(sceneimage,Degree,500,400,500);

	cout<<"\nTotal landmarks: "<<esLandmarks.size();
	/*//image.cannyAlgorithm();
	vector<ptr_Line> lines = image.getApproximateLines(3);

	PHoughTransform phTransform;
	phTransform.setRefPoint(new Point(1632, 1224));
	phTransform.constructPHTTable(lines);

	//shapeHistogram(image,Degree,500);

	Image simage("data/Md_046.jpg");
	vector<ptr_Line> line2 = simage.getApproximateLines(3);

	int width = simage.getGrayMatrix()->getCols();
	int height = simage.getGrayMatrix()->getRows();

	vector<ptr_Line> maxVector;
	ptr_PHTEntry pht = matchingInScene(phTransform.getPHTEntries(), line2, width,
		height, maxVector);
	cout << "\nTotal entries: " << maxVector.size();
	cout << "\n" << maxVector[0]->getBegin()->getX() << "\t"
		<< maxVector[0]->getBegin()->getY() << "\t"
		<< maxVector[0]->getEnd()->getX() << "\t" << maxVector[0]->getEnd()->getY();
	cout << "\n" << maxVector[1]->getBegin()->getX() << "\t"
		<< maxVector[1]->getBegin()->getY() << "\t"
		<< maxVector[1]->getEnd()->getX() << "\t" << maxVector[1]->getEnd()->getY();

	double angleDiff = 0;
	refPointInScene(pht, maxVector, angleDiff, image.getListOfManualLandmarks(),
		width, height);

	ptr_Point ePoint;
	vector<ptr_Point> esLandmarks = estimateLandmarks(image, simage, angleDiff,
		ePoint);
	cout << "\nTotal estimated landmarks: " << esLandmarks.size();
	for (int k = 0; k < esLandmarks.size(); k++)
	{
		ptr_Point p = esLandmarks.at(k);
		cout << "\nCoordinate: " << p->getX() << "\t" << p->getY();
	}
	//ptr_IntMatrix sRotate = simage.rotate(ePoint, angleDiff, 1);

	verifyLandmarks(image, simage, image.getListOfManualLandmarks(), esLandmarks,
		400, 500, angleDiff, ePoint);*/


	cout << endl << "finish\n";
	return 0;
}
