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
#include <cstdlib>
#include <dirent.h>

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

#include <QtGui/QApplication>
#include "ui/ImageViewer.h"

#include "MAELab.h"

/*
 int main(int argc, char* argv[])
 {
 //cout.precision(16);
 cout << "MAELab test" << endl << endl;
 string mImagePath, mLMPath, sImagePath;
 int distanceAcc = 500; // distance accuracy to compute the geometric histgoram
 int tempSize = 400; // template size uses in template matching
 int sceneSize = 500; // image size uses in template matching
 if (argc != 3)
 {
 mImagePath = "data/Md039.JPG";
 mLMPath = "data/Md 039.TPS";
 sImagePath = "data/sceneImages/Md_046.jpg";
 }
 else
 {
 mImagePath = argv[0];
 mLMPath = argv[1];
 sImagePath = argv[2];
 }

 Image modelImage(mImagePath);
 modelImage.readManualLandmarks(mLMPath);
 Image sceneimage(sImagePath);

 LandmarkDetection lm;
 ptr_Treatments tr = new LandmarkDetection();
 tr->setRefImage(modelImage);

 vector<ptr_Point> esLandmarks = estimatedLandmarks(tr, sceneimage, Degree,
 distanceAcc, tempSize, sceneSize);

 cout << "\nTotal landmarks: " << esLandmarks.size();

 cout << endl << "finish\n";
 return 0;
 }
 */
int main(int argc, char* argv[])
{
	cout << "\n MAELab with graphic user interface !!!" << endl;
	QApplication app(argc, argv);
	ImageViewer imageViewer;
#if defined(Q_OS_SYMBIAN)
	imageViewer.showMaximized();
#else
	imageViewer.show();
#endif
	return app.exec();
	return 0;

}
