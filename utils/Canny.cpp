/*
 * Canny.cpp
 *
 *  Created on: Oct 9, 2016
 *      Author: linh
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
#include "../imageModel/Matrix.h"
#include "../imageModel/Point.h"
#include "../io/Reader.h"
#include "Canny.h"

const int gaussianMask[5][5] = {
		{ 2, 4, 5, 4, 2 },
		{ 4, 9, 12, 9, 4 },
		{ 5, 12, 15, 12, 5 },
		{ 4, 9, 12, 9, 4 },
		{ 2, 4, 5, 4, 2 }
};
const int GMASKTOTAL = 159;
const int gxMask[3][3] = {
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 }
};
const int gyMask[3][3] = {
		{ 1, 2, 1 },
		{ 0, 0, 0 },
		{ -1, -2, -1 }
};
const float PI = 3.14159;

const float ANGLE_0 = 0.0;
const float ANGLE_45 = 45.0;
const float ANGLE_90 = 90.0;
const float ANGLE_135 = 135.0;
const float ANGLE_180 = 180.0;

const float LIMIT_ANGLE_1 = 22.5;
const float LIMIT_ANGLE_2 = 67.5;
const float LIMIT_ANGLE_3 = 112.5;
const float LIMIT_ANGLE_4 = 157.5;

const unsigned int WHITE = 255;
const unsigned int BLACK = 0;



ptr_IntMatrix gaussianBlur(ptr_IntMatrix img) {
	ptr_IntMatrix returnImg = new Matrix<unsigned int>(img->getRows(),
			img->getCols(), 0);
	unsigned int newPixel = 0;

	for (int r = 2; r < img->getRows() - 2; r++) {
		for (int c = 2; c < img->getCols() - 2; c++) {
			newPixel = 0;
			for (int rg = -2; rg <= 2; rg++) {
				for (int cg = -2; cg <= 2; cg++) {
					newPixel += img->getAtPosition(r+rg,c+cg) * gaussianMask[2 + rg][2+cg];
				}

			}
			int value = newPixel / GMASKTOTAL;
			returnImg->setAtPosition(r, c, value);
		}

	}

	return returnImg;

}
ptr_IntMatrix edgeDirection(ptr_IntMatrix img, ptr_IntMatrix &gradient) {
	ptr_IntMatrix edgeDir = new Matrix<unsigned int>(img->getRows(),
			img->getCols(), 0);
	float thisAngle, newAngle;

	int gx, gy, rowTotal, colTotal;
	for (int r = 1; r < img->getRows()-1; r++) {
		for (int c = 1; c < img->getCols()-1; c++) {
			gx = 0;
			gy = 0;
			for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
				for (int colOffset = -1; colOffset <= 1; colOffset++) {
					rowTotal = r + rowOffset;
					colTotal = c + colOffset;
					gx += img->getAtPosition(rowTotal, colTotal)
							* gxMask[rowOffset + 1][colOffset + 1];
					gy += img->getAtPosition(rowTotal, colTotal)
							* gyMask[rowOffset + 1][colOffset + 1];
				}
			}

			gradient->setAtPosition(r, c, sqrt(pow(gx, 2.0) + pow(gy, 2.0)));
			thisAngle = (atan2(gx, gy) / PI) * ANGLE_180;

			if (((thisAngle < LIMIT_ANGLE_1) && (thisAngle > -LIMIT_ANGLE_1))
					|| (thisAngle > LIMIT_ANGLE_4) || (thisAngle < -LIMIT_ANGLE_4))
				newAngle = ANGLE_0;
			if (((thisAngle > LIMIT_ANGLE_1) && (thisAngle < LIMIT_ANGLE_2))
					|| ((thisAngle < -LIMIT_ANGLE_3) && (thisAngle > -LIMIT_ANGLE_4)))
				newAngle = ANGLE_45;
			if (((thisAngle > LIMIT_ANGLE_2) && (thisAngle < LIMIT_ANGLE_3))
					|| ((thisAngle < -LIMIT_ANGLE_2) && (thisAngle > -LIMIT_ANGLE_3)))
				newAngle = ANGLE_90;
			if (((thisAngle > LIMIT_ANGLE_3) && (thisAngle < LIMIT_ANGLE_4))
					|| ((thisAngle < -LIMIT_ANGLE_1) && (thisAngle > -LIMIT_ANGLE_2)))
				newAngle = ANGLE_135;

			edgeDir->setAtPosition(r, c, newAngle);
		}
	}

	return edgeDir;
}

void findEdge(ptr_IntMatrix &img, ptr_IntMatrix edgeDir, ptr_IntMatrix gradient,
		int rowShift, int colShift, int row, int col, float dir,
		int lowerThreshold) {

	int newRow = 0, newCol = 0;
	bool edgeEnd = false;
	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			edgeEnd = true;
	} else {
		if (col < img->getCols() - 1)
			newCol = col + colShift;
		else
			edgeEnd = true;
	}
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	} else {
		if (row < img->getRows() - 1)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	}
	while (edgeDir->getAtPosition(newRow, newCol) == dir && !edgeEnd
			&& gradient->getAtPosition(newRow, newCol) > lowerThreshold) {

		img->setAtPosition(newRow, newCol, WHITE);
		if (colShift < 0) {
			if (newCol > 0)
				newCol = newCol + colShift;
			else
				edgeEnd = true;
		} else {
			if (newCol < img->getCols() - 1)
				newCol = newCol + colShift;
			else
				edgeEnd = true;
		}

		if (rowShift < 0) {
			if (newRow > 0)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		} else {
			if (newRow < img->getRows() - 1)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		}
	}
}

void traceEdge(ptr_IntMatrix &img, ptr_IntMatrix gradient,
		ptr_IntMatrix edgeDir, int upperThreshold, int lowerThreshold) {
	//bool edgeEnd = false;
	for (int r = 1; r < img->getRows() - 1; r++) {
		for (int c = 1; c < img->getCols() - 1; c++) {
			//edgeEnd = false;
			if (gradient->getAtPosition(r, c) > upperThreshold) {
				switch (edgeDir->getAtPosition(r, c)) {
				case 0:
					findEdge(img, edgeDir, gradient, 0, 1, r, c, ANGLE_0,
							lowerThreshold);
					break;
				case 45:
					findEdge(img, edgeDir, gradient, 1, 1, r, c, ANGLE_45,
							lowerThreshold);
					break;
				case 90:
					findEdge(img, edgeDir, gradient, 1, 0, r, c, ANGLE_90,
							lowerThreshold);
					break;
				case 135:
					findEdge(img, edgeDir, gradient, 1, -1, r, c, ANGLE_135,
							lowerThreshold);
					break;
				default:
					img->setAtPosition(r, c, BLACK);
					break;
				}
			} else
				img->setAtPosition(r, c, BLACK);
		}
	}
	for (int r = 0; r < img->getRows(); r++) {
		for (int c = 0; c < img->getCols(); c++) {
			if (img->getAtPosition(r, c) != WHITE
					&& img->getAtPosition(r, c) != BLACK)
				img->setAtPosition(r, c, BLACK);
		}
	}

}

void suppressNonMax(ptr_IntMatrix &img, ptr_IntMatrix edgeDir,
		ptr_IntMatrix gradient, int rowShift, int colShift, int row, int col,
		int dir, int lowerThreshold) {
	bool edgeEnd = false;
	float nonMax[img->getRows()][3]; // temporarily stores gradient and position of pixels in parallel edges
	int pixelCount = 0; // store the number of pixels in parallel edges
	int count;
	//int max[3] = { 0 }; // maximum point in a wide edge
	int newRow, newCol;

	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			edgeEnd = true;
	} else {
		if (col < img->getCols() - 1)
			newCol = col + colShift;
		else
			edgeEnd = true;
	}
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	} else {
		if (row < img->getRows() - 1)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	}
	while ((int)edgeDir->getAtPosition(newRow, newCol) == dir && !edgeEnd
			&& img->getAtPosition(newRow, newCol) == WHITE) {
		if (colShift < 0) {
			if (newCol > 0)
				newCol = newCol + colShift;
			else
				edgeEnd = true;
		} else {
			if (newCol < img->getCols() - 1)
				newCol = newCol + colShift;
			else
				edgeEnd = true;
		}

		if (rowShift < 0) {
			if (newRow > 0)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		} else {
			if (newRow < img->getRows() - 1)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		}
		// save the position and gradient of pixels in parallel edges
		nonMax[pixelCount][0] = newRow; // row index
		nonMax[pixelCount][1] = newCol; // column index
		nonMax[pixelCount][2] = gradient->getAtPosition(newRow, newCol); // gradient
		pixelCount++;

	}
/*
	edgeEnd = false;
	colShift *= -1;
	rowShift *= -1;
	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			edgeEnd = true;
	} else {
		if (col < img->getCols() - 1)
			newCol = col + colShift;
		else
			edgeEnd = true;
	}
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	} else {
		if (row < img->getRows() - 1)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	}

	while ((int)edgeDir->getAtPosition(newRow, newCol) == dir && !edgeEnd
			&& img->getAtPosition(newRow, newCol) == WHITE) {
		if (colShift < 0) {
			if (newCol > 0)
				newCol = newCol + colShift;
			else
				edgeEnd = true;
		} else {
			if (newCol < img->getCols() - 1)
				newCol = newCol + colShift;
			else
				edgeEnd = true;
		}

		if (rowShift < 0) {
			if (newRow > 0)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		} else {
			if (newRow < img->getRows() - 1)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		}
		// save the position and gradient of pixels in parallel edges
		nonMax[pixelCount][0] = newRow;
		nonMax[pixelCount][1] = newCol;
		nonMax[pixelCount][2] = gradient->getAtPosition(newRow, newCol);
		pixelCount++;

	}*/

	//Suppress non-maximum edges
	/*for (count = 0; count < pixelCount; count++) {
		if (nonMax[count][2] > max[2]) {
			max[0] = nonMax[count][0];
			max[1] = nonMax[count][1];
			max[2] = nonMax[count][2];
		}
	}*/
	for (int count = 0; count < pixelCount; count++) {
		img->setAtPosition(nonMax[count][0], nonMax[count][1], BLACK);
	}

}
void suppress(ptr_IntMatrix img, ptr_IntMatrix edgeDir, ptr_IntMatrix gradient,
		int lowerThreshold) {
	for (int r = 1; r < img->getRows() - 1; r++) {
		for (int c = 1; c < img->getCols() - 1; c++) {
			if (img->getAtPosition(r, c) == WHITE) {
				switch (edgeDir->getAtPosition(r, c)) {
				case 0:
					suppressNonMax(img, edgeDir, gradient, 1, 0, r, c, ANGLE_0,
							lowerThreshold);
					break;
				case 45:
					suppressNonMax(img, edgeDir, gradient, 1, -1, r, c, ANGLE_45,
							lowerThreshold);
					break;
				case 90:
					suppressNonMax(img, edgeDir, gradient, 0, 1, r, c, ANGLE_90,
							lowerThreshold);
					break;
				case 135:
					suppressNonMax(img, edgeDir, gradient, 1, 1, r, c,ANGLE_135,
							lowerThreshold);
					break;
				default:
					break;
				}
			}
		}
	}
}
void canny(ptr_IntMatrix inputImg, ptr_IntMatrix outputImg, int lowerThreshold,
		int upperThreshold) {
	// co the detect duoc list of edge hay khong ??????
	ptr_IntMatrix gaussianMatrix = gaussianBlur(inputImg);
	ptr_IntMatrix edgeDir, gradient;
	gradient = new Matrix<unsigned int>(inputImg->getRows(),inputImg->getCols(),0);
	edgeDir = edgeDirection(gaussianMatrix,gradient);
	traceEdge(gaussianMatrix,gradient,edgeDir,upperThreshold,lowerThreshold);
	suppress(gaussianMatrix,edgeDir,gradient,lowerThreshold);

	//suppress some border lines
	for (int r = 0; r < gaussianMatrix->getRows(); r++) {
		for (int c = 0; c < gaussianMatrix->getCols(); c++) {
			if(r < 5 || r > gaussianMatrix->getRows() - 5
				|| c < 5 || c > gaussianMatrix->getCols() - 5)
				gaussianMatrix->setAtPosition(r,c,0);
		}
	}

	saveGrayJPG(gaussianMatrix,gaussianMatrix->getCols(),gaussianMatrix->getRows(),"output/test.jpg");
}
