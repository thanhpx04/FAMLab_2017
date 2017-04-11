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
#include "utils/Drawing.h"
#include "io/Reader.h"

#include "segmentation/Thresholds.h"
#include "segmentation/Canny.h"
#include "segmentation/Filters.h"


/**
 * This function is used to fill the hole inside the object (pronotum)
 * Input: file path of rgb image
 * Output: the output image under Binary format.
 */

void holeFill(string filename, string savename)
{
	Image image(filename);
	Matrix<double> gauKernel = getGaussianKernel(5, 1);
	Matrix<int> result = gaussianBlur(*image.getGrayMatrix(), gauKernel);
	ptr_IntMatrix binMatrix = binaryThreshold(&result,image.getThresholdValue(),255);
	binMatrix= postProcess(binMatrix,255);
	saveGrayScale(savename.c_str(),binMatrix);
}

int main(int argc, char* argv[])
{
	cout << "\n Test a function !!!" << endl;

	// ================================================================ Test hole fill =================================================
	cout<<endl << "\n Hole fill"<<endl;
	string filename, savename;
	if(argc == 1)
	{
		cout<<"\nWithout parameters !!"<<endl;
		filename = "data/md028.jpg";
		savename = "results/md028.jpg";
	}
	else
	{
		cout<<"\nWith parameters !!"<<endl;
		filename = argv[1];
		savename = argv[2];
	}
	holeFill(filename,savename);

}
