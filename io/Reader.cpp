#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

#include "../imageModel/Matrix.h"
#include "../imageModel/Point.h"

#include "TPSReader.h"
#include "LoadJPG.h"
#include "SaveJPG.h"

#include "Reader.h"

//====================================================== JPEG File ==================================================
unsigned char* decodeJPG(const char* filename, unsigned int &width,
		unsigned int &height) {

	unsigned int lengthOfFile;
	unsigned char* buf = readFile((char*) filename, lengthOfFile);

	unsigned char* rgbpix = NULL;
	width = 0;
	height = 0;
	DecodeJpgFileData(buf, lengthOfFile, &rgbpix, &width, &height);
	delete[] buf;

	return rgbpix;
}
ptr_RGBMatrix readJPGToRGB(const char* filename) {
	ptr_RGBMatrix rgbMatrix;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* rgbpix = decodeJPG(filename, width, height);
	if (rgbpix == NULL) {
		cout << endl << ("Failed to decode jpg\n");
		return 0;
	}

	rgbMatrix = new Matrix<RGB>(height, width);
	cout <<dec<< "width: " << rgbMatrix->getCols() << "\t height: "
			<< rgbMatrix->getRows();
	for (unsigned int h = 0; h < height; h++) { // rows
		for (unsigned  int w = 0; w < width; w++) { // cols
			unsigned int i = (w + width * h) * 3;
			RGB rgb;
			rgb.R = (unsigned short int) rgbpix[i];
			rgb.G = (unsigned short int) rgbpix[i + 1];
			rgb.B = (unsigned short int) rgbpix[i + 2];
			rgbMatrix->setAtPosition(h, w, rgb);
		}
	}

	//cout<<"\nTest value: "<<(unsigned short int)rgbpix[0]<<"\t"<<(unsigned short int)rgbpix[1]<<"\t"<<(unsigned short int)rgbpix[2];
	//cout<<"\nTest value matrix: "<<rgbMatrix->getAtPosition(0,0).R<<"\t"<<rgbMatrix->getAtPosition(0,0).G<<"\t"<<rgbMatrix->getAtPosition(0,0).B;
	delete[] rgbpix;
	return rgbMatrix;
}

ptr_IntMatrix readJPGToGray(const char* filename) {
	ptr_IntMatrix grayMatrix;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* rgbpix = decodeJPG(filename, width, height);

	if (rgbpix == NULL) {
		cout << endl << ("Failed to decode jpg\n");
		return 0;
	}

	grayMatrix = new Matrix<unsigned int>(height, width);
	for (unsigned int h = 0; h < height; h++) {
		for (unsigned int w = 0; w < width; w++) {
			int i = (w + width * h) * 3;
			grayMatrix->setAtPosition(h, w,
					((rgbpix[i] + rgbpix[i + 1] + rgbpix[i + 2]) / 3));
		}
	}

	delete[] rgbpix;
	return grayMatrix;
}


void saveRGBJPG(ptr_RGBMatrix matrix, int width, int height,
		char* outputFilename) {
	unsigned char *rgbBuff = new unsigned char[width * height * 3];

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			RGB pixel = matrix->getAtPosition(r, c);
			int i = (c + width * r) * 3;
			(rgbBuff[i]) = (unsigned char) pixel.R;
			(rgbBuff[i + 1]) = (unsigned char) pixel.G;
			(rgbBuff[i + 2]) = (unsigned char) pixel.B;

		}

	}
	WriteBMP24("output/abc.bmp", width, height, rgbBuff);
	SaveJpgFile((char*)"output/abc.bmp", outputFilename);
	remove("output/abc.bmp");
}
void saveGrayJPG(ptr_IntMatrix grayMatrix, int width, int height,
		char* outputFileName) {
	unsigned char *rgbBuff = new unsigned char[width * height * 3];
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			int pixel = grayMatrix->getAtPosition(r, c);
			int i = (c + width * r) * 3;
			(rgbBuff[i]) = pixel;
			(rgbBuff[i + 1]) = pixel;
			(rgbBuff[i + 2]) = pixel;

		}

	}
	WriteBMP24("output/temp.bmp", width, height, rgbBuff);
	delete[] rgbBuff;
	SaveJpgFile((char*)"output/temp.bmp", outputFileName);
	remove("output/temp.bmp");
}
// ============================================================== TPS File =====================================================
vector<ptr_Point> readTPSFile(const char* filename) {
	return readTPS(filename);
}

