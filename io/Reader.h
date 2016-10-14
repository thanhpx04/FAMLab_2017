/*
 * Reader.h
 *
 *  Created on: Oct 6, 2016
 *      Author: linh
 */

#ifndef READER_H_
#define READER_H_

ptr_RGBMatrix readJPGToRGB(const char* filename);
ptr_IntMatrix readJPGToGray(const char* filename);
//ptr_IntMatrix convertRGBToGray(ptr_RGBMatrix rgbMatrix);
vector<ptr_Point> readTPSFile(const char* filename);
void saveRGBJPG(ptr_RGBMatrix matrix, int width, int height,
		char* outputFilename);
void saveGrayJPG(ptr_IntMatrix grayMatrix, int width, int height,
		char* outputFileName);
#endif /* READER_H_ */
