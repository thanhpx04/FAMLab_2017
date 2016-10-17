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

// gaussian mask
const int gaussianMask[5][5] = { { 2, 4, 5, 4, 2 }, { 4, 9, 12, 9, 4 }, { 5, 12,
		15, 12, 5 }, { 4, 9, 12, 9, 4 }, { 2, 4, 5, 4, 2 } };
// sum of values in gaussian mask
const int GMASKTOTAL = 159;

// Sobel masks
const int gxMask[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
const int gyMask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
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

/*
 * Apply a Gaussian blur
 * Input: - input image matrix (gray)
 * Output: - matrix after bluring with Gaussian mask
 */
ptr_IntMatrix gaussianBlur(ptr_IntMatrix img) {
	ptr_IntMatrix returnImg = new Matrix<unsigned int>(img->getRows(),
			img->getCols(), 0);
	unsigned int newPixel = 0;

	for (int r = 2; r < img->getRows() - 2; r++) {
		for (int c = 2; c < img->getCols() - 2; c++) {
			newPixel = 0;
			for (int rg = -2; rg <= 2; rg++) {
				for (int cg = -2; cg <= 2; cg++) {
					newPixel += img->getAtPosition(r + rg, c + cg)
							* gaussianMask[2 + rg][2 + cg];
				}

			}
			int value = newPixel / GMASKTOTAL;
			returnImg->setAtPosition(r, c, value);
		}

	}

	return returnImg;

}

/*
 * Detect the direction of edge  with four possible values.
 * Input: - image matrix (gray) after applying Gaussian
 * 		  - gradient matrix (same size with matrix image), it uses to save the gradient strength of each pixel
 * Output:- edge direction matrix (same size with image matrix), it stores the edge direction of each pixel
 */
ptr_IntMatrix edgeDirection(ptr_IntMatrix img, ptr_IntMatrix &gradient) {
	ptr_IntMatrix edgeDir = new Matrix<unsigned int>(img->getRows(),
			img->getCols(), 0);
	float thisAngle, newAngle;

	int gx, gy, rowTotal, colTotal;
	for (int r = 1; r < img->getRows() - 1; r++) {
		for (int c = 1; c < img->getCols() - 1; c++) {
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
					|| (thisAngle > LIMIT_ANGLE_4)
					|| (thisAngle < -LIMIT_ANGLE_4))
				newAngle = ANGLE_0;
			if (((thisAngle > LIMIT_ANGLE_1) && (thisAngle < LIMIT_ANGLE_2))
					|| ((thisAngle < -LIMIT_ANGLE_3)
							&& (thisAngle > -LIMIT_ANGLE_4)))
				newAngle = ANGLE_45;
			if (((thisAngle > LIMIT_ANGLE_2) && (thisAngle < LIMIT_ANGLE_3))
					|| ((thisAngle < -LIMIT_ANGLE_2)
							&& (thisAngle > -LIMIT_ANGLE_3)))
				newAngle = ANGLE_90;
			if (((thisAngle > LIMIT_ANGLE_3) && (thisAngle < LIMIT_ANGLE_4))
					|| ((thisAngle < -LIMIT_ANGLE_1)
							&& (thisAngle > -LIMIT_ANGLE_2)))
				newAngle = ANGLE_135;

			edgeDir->setAtPosition(r, c, newAngle);
		}
	}

	return edgeDir;
}

/*
 * Indicate the next pixel in edge
 * Input:	- column index (currently column)
 * 			- row index
 * 			- number of columns shift
 * 			- number of rows shift
 * 			- number of columns of matrix
 * 			- number of rows of matrix
 * 			- new column index
 * 			- new row index
 * Output:  - the end of edge or not
 */
bool shiftPosition(int col, int row, int colShift, int rowShift, int cols,
		int rows, int &newCol, int &newRow) {
	bool end = false;
	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			end = true;
	} else {
		if (col < cols - 1)
			newCol = col + colShift;
		else
			end = true;
	}
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			end = true;
	} else {
		if (row < rows - 1)
			newRow = row + rowShift;
		else
			end = true;
	}
	return end;
}

/*
 * Find the edge in image matrix
 * Input:  - input image matrix
 * 		   - edge direction matrix
 * 		   - gradient strength matrix
 * 		   - number of row shift
 * 		   - number of column shift
 * 		   - row index
 * 		   - column index
 * 		   - angle direction value
 * 		   - lower threshold value
 * Output: - update the value of input pixels followed edge detected.
 */
void findEdge(ptr_IntMatrix &img, ptr_IntMatrix edgeDir, ptr_IntMatrix gradient,
		int rowShift, int colShift, int row, int col, float dir,
		int lowerThreshold) {

	int newRow = 0, newCol = 0;
	bool edgeEnd = shiftPosition(col, row, colShift, rowShift, img->getCols(),
			img->getRows(), newCol, newRow);

	while (edgeDir->getAtPosition(newRow, newCol) == dir && !edgeEnd
			&& gradient->getAtPosition(newRow, newCol) > lowerThreshold) {

		img->setAtPosition(newRow, newCol, WHITE);

		edgeEnd = shiftPosition(newCol, newRow, colShift, rowShift,
				img->getCols(), img->getRows(), newCol, newRow);

	}
}

/*
 * Trace all edge in image matrix
 * Input:  - image matrix
 * 		   - gradient strength matrix
 * 		   - edge direction matrix
 * 		   - upper threshold value
 * 		   - lower threshold value
 * Output: - trace all the edges in the input image matrix
 */
void traceEdge(ptr_IntMatrix &img, ptr_IntMatrix gradient,
		ptr_IntMatrix edgeDir, int upperThreshold, int lowerThreshold) {

	for (int r = 1; r < img->getRows() - 1; r++) {
		for (int c = 1; c < img->getCols() - 1; c++) {

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

/*
 * Suppress the weak edge( or parallel edge)
 * Input:  - input image matrix
 * 		   - edge direction matrix
 * 		   - gradient strength matrix
 * 		   - number of rows shift
 * 		   - number of columns shift
 * 		   - row index
 * 		   - column index
 * 		   - angle direction value
 * Output: - suppress the weak edge in input image matrix
 */
void suppressNonMax(ptr_IntMatrix &img, ptr_IntMatrix &edgeDir,
		ptr_IntMatrix gradient, int rowShift, int colShift, int row, int col,
		int dir) {
	bool edgeEnd = false;
	float nonMax[img->getRows()][3]; // temporarily stores gradient and position of pixels in parallel edges
	int pixelCount = 0; // store the number of pixels in parallel edges
	int count;
	//int max[3] = { 0 }; // maximum point in a wide edge
	int newRow, newCol;

	edgeEnd = shiftPosition(col, row, colShift, rowShift, img->getCols(),
			img->getRows(), newCol, newRow);
	while ((int) edgeDir->getAtPosition(newRow, newCol) == dir && !edgeEnd
			&& img->getAtPosition(newRow, newCol) == WHITE) {

		edgeEnd = shiftPosition(newCol, newRow, colShift, rowShift,
				img->getCols(), img->getRows(), newCol, newRow);

		// save the position and gradient of pixels in parallel edges
		nonMax[pixelCount][0] = newRow; // row index
		nonMax[pixelCount][1] = newCol; // column index
		nonMax[pixelCount][2] = gradient->getAtPosition(newRow, newCol); // gradient

		pixelCount++;

	}

	for (int count = 0; count < pixelCount; count++) {
		img->setAtPosition(nonMax[count][0], nonMax[count][1], BLACK);
		edgeDir->setAtPosition(nonMax[count][0], nonMax[count][1], ANGLE_180);
		//gradient->setAtPosition(nonMax[count][0], nonMax[count][1], 0);
	}

}

/*
 * Suppress all of weak edges in input image matrix
 * Input:  - input image matrix
 * 		   - edge direction matrix
 * 		   - gradient strength matrix
 * Outuput:- supress the weak edges in input image matrix
 */

void suppress(ptr_IntMatrix &img, ptr_IntMatrix &edgeDir,
		ptr_IntMatrix gradient) {
	for (int r = 1; r < img->getRows() - 1; r++) {
		for (int c = 1; c < img->getCols() - 1; c++) {
			if (img->getAtPosition(r, c) == WHITE) {
				switch (edgeDir->getAtPosition(r, c)) {
				case 0:
					suppressNonMax(img, edgeDir, gradient, 1, 0, r, c, ANGLE_0);
					break;
				case 45:
					suppressNonMax(img, edgeDir, gradient, 1, -1, r, c,
							ANGLE_45);
					break;
				case 90:
					suppressNonMax(img, edgeDir, gradient, 0, 1, r, c,
							ANGLE_90);
					break;
				case 135:
					suppressNonMax(img, edgeDir, gradient, 1, 1, r, c,
							ANGLE_135);
					break;
				default:
					break;
				}
			}
		}
	}
}

/*
 *  Get an edge in after finished all operations
 *  Input:  - input image matrix
 *  		- edge direction matrix
 *  		- number of rows shift
 *  		- number of columns shift
 *  		- row index
 *  		- column index
 *  		- angle direction value
 *  Output: - the list of point presented for an edge
 */
vector<ptr_Point> getEdge(ptr_IntMatrix img, ptr_IntMatrix edgeDir,
		int rowShift, int colShift, int row, int col, int dir) {

	vector<ptr_Point> edge;
	edge.push_back(new Point(col, row));
	bool edgeEnd = false;

	int newRow, newCol;

	edgeEnd = shiftPosition(col, row, colShift, rowShift, img->getCols(),
			img->getRows(), newCol, newRow);
	while (!edgeEnd && img->getAtPosition(newRow, newCol) == WHITE) {
		edgeEnd = shiftPosition(newCol, newRow, colShift, rowShift,
				img->getCols(), img->getRows(), newCol, newRow);
		edge.push_back(new Point(newCol, newRow));

	}
	return edge;
}

/*
 * Get all edge of image
 * Input:   - input image matrix
 * 			- edge direction matrix
 * Output: - list of edges in image. Each each is presented by a list of points
 */
vector<vector<ptr_Point> > getEdges(ptr_IntMatrix img, ptr_IntMatrix edgeDir) {

	vector<vector<ptr_Point> > edges;
	vector<ptr_Point> edge;
	for (int r = 0; r < img->getRows(); r++) {
		for (int c = 0; c < img->getCols(); c++) {
			if (img->getAtPosition(r, c) == WHITE) {
				switch (edgeDir->getAtPosition(r, c)) {
				case 0:
					edge = getEdge(img, edgeDir, 0, 1, r, c, ANGLE_0);
					break;
				case 45:
					edge = getEdge(img, edgeDir, 1, 1, r, c, ANGLE_45);
					break;
				case 90:
					edge = getEdge(img, edgeDir, 1, 0, r, c, ANGLE_90);
					break;
				case 135:
					edge = getEdge(img, edgeDir, 1, -1, r, c, ANGLE_135);
					break;
				default:
					break;
				}
				edges.push_back(edge);
			}
		}
	}
	return edges;
}

/*
 * Apply Canny algorithm and return list of edges
 * Input: - image matrix (gray)
 * 		  - lower threshold value
 * 		  - upper threshold value
 * Output:- list of edges which extracted from Canny
 */
vector<vector<ptr_Point> > canny(ptr_IntMatrix inputImg, int lowerThreshold,
		int upperThreshold) {

	ptr_IntMatrix gaussianMatrix = gaussianBlur(inputImg);
	ptr_IntMatrix edgeDir, gradient;
	gradient = new Matrix<unsigned int>(inputImg->getRows(),
			inputImg->getCols(), 0);
	edgeDir = edgeDirection(gaussianMatrix, gradient);
	traceEdge(gaussianMatrix, gradient, edgeDir, upperThreshold,
			lowerThreshold);

	suppress(gaussianMatrix, edgeDir, gradient);

	//suppress some border lines
	for (int r = 0; r < gaussianMatrix->getRows(); r++) {
		for (int c = 0; c < gaussianMatrix->getCols(); c++) {
			if (r < 5 || r > gaussianMatrix->getRows() - 5 || c < 5
					|| c > gaussianMatrix->getCols() - 5)
				gaussianMatrix->setAtPosition(r, c, 0);
		}
	}

	/*saveGrayJPG(gaussianMatrix, gaussianMatrix->getCols(),
	 gaussianMatrix->getRows(), "output/test2.jpg");*/

	vector<vector<ptr_Point> > edges = getEdges(gaussianMatrix, edgeDir);

	/*ptr_IntMatrix test = new Matrix<unsigned int>(inputImg->getRows(),
	 inputImg->getCols(), 0);
	 cout << endl << "Number of edges: " << dec << edges.size();
	 for (size_t i = 0; i < edges.size(); i++) {
	 vector<ptr_Point> edge = edges.at(i);
	 for (size_t j = 0; j < edge.size(); j++) {
	 ptr_Point p = edge.at(j);
	 test->setAtPosition(p->getY(), p->getX(), WHITE);
	 }
	 }
	 saveGrayJPG(test, test->getCols(), test->getRows(), "output/test3.jpg");*/
	return edges;
}
