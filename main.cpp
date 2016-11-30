/*
 *
 *
 * Test file
 */
#include <iostream>
#include <math.h>
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


int main() {
	cout.precision(20);
	cout << "MAELab test" << endl << endl;

	//Image image("/home/linh/Desktop/Temps/md/images/Md 009.JPG");
	Image image("data/Md039.JPG");
	//image.cannyAlgorithm();
	//vector<ptr_Line> lines = image.getApproximateLines(3);
	//shapeHistogram(image,Degree,500);

	Image simage("data/Md_046.jpg");


	ShapeHistogram shapeHist;
	//shapeHist.shapeHistogram(image,Degree,500);
	double bhatMeasure = shapeHist.bhattacharyyaDistance(image,simage,Degree,500);
	cout<<"\nBhattacharyya metric: "<< bhatMeasure;

	cout << endl << "finish\n";
	return 0;
}
