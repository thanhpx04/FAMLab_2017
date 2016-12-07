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
/*
 #include "io/TPSReader.h"
 #include "io/LoadJPG.h"
 */
/*
 #include "io/Reader.h"
 #include "io/JPEGReader.h"
 #include "imageModel/Image.h"
 #include "utils/Canny.h"
 #include "utils/Suzuki.h"*/
#include "histograms/ShapeHistogram.h"
#include "pht/PHTEntry.h"
#include "pht/PHoughTransform.h"

#include "pointInterest/Treatments.h"
#include "pointInterest/ProHoughTransform.h"
#include "correlation/CrossCorrelation.h"

int main()
{
	//cout.precision(16);
	cout << "MAELab test" << endl << endl;

	//Image image("/home/linh/Desktop/Temps/md/images/Md 009.JPG");
	Image image("data/Md039.JPG");
	image.readManualLandmarks("/home/linh/Desktop/Temps/md/landmarks/Md 039.TPS");
	//image.cannyAlgorithm();
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
		400, 500, angleDiff, ePoint);

//ShapeHistogram shapeHist;
//shapeHist.shapeHistogram(image,Degree,500);
//double bhatMeasure = shapeHist.bhattacharyyaDistance(image,simage,Degree,500);
//cout<<"\nBhattacharyya metric: "<< bhatMeasure;

	cout << endl << "finish\n";
	return 0;
}
