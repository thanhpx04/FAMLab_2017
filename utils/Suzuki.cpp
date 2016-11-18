#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "../imageModel/Matrix.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../io/Reader.h"
#include "Suzuki.h"

/*
 * The direction of neighbor pixel
 */
enum Direction {
	EAST = 1,
	NORTHEAST = 2,
	NORTH = 3,
	NORTHWEST = 4,
	WEST = 5,
	SOUTHWEST = 6,
	SOUTH = 7,
	SOUTHEAST = 8
};

/*
 * Indicate the direction of a neighbor pixel (ni,nj)
 * of a pixel (i,j)
 */
Direction getPosition(int i, int j, int ni, int nj) {

	if (i == ni && j < nj)
		return EAST;
	if (i > ni && j < nj)
		return NORTHEAST;
	if (i > ni && j == nj)
		return NORTH;
	if (i > ni && j > nj)
		return NORTHWEST;
	if (i == ni && j > nj)
		return WEST;
	if (i < ni && j > nj)
		return SOUTHWEST;
	if (i < ni && j == nj)
		return SOUTH;
	if (i < ni && j < nj)
		return SOUTHEAST;
	return SOUTHEAST;
}

void clockWiseCheck(ptr_IntMatrix inputImage, int i, int j, int ci, int cj,
		int &iOut, int &jOut) {
	int rows = inputImage->getRows();
	int cols = inputImage->getCols();

	if (inputImage->getAtPosition(ci, cj) != 0) {
		iOut = ci;
		jOut = cj;
		return;
	} else {
		Direction pos = getPosition(i, j, ci, cj);

		if (pos == EAST) {
			for (int k = j + 1; k >= j - 1; k--) {
				if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0
						&& inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
			if (j - 1 >= 0 && j - 1 < cols
					&& inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;
				return;
			}
			for (int k = j - 1; k <= j + 1; k++) {
				if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows
						&& inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}

		}
		if (pos == SOUTHEAST) {
			if (i + 1 < rows && i + 1 >= 0
					&& inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
			for (int r = i + 1; r >= i - 1; r--) {
				if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols
						&& inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}
			if (i - 1 >= 0 && inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}
			for (int r = i - 1; r < i + 1; r++) {
				if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0
						&& inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;
					return;
				}
			}
		}

		if (pos == SOUTH) {
			for (int r = i + 1; r >= i - 1; r--) {
				if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols
						&& inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}
			if (i - 1 >= 0 && inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}
			for (int r = i - 1; r <= i + 1; r++) {
				if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0
						&& inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;
					return;
				}
			}
		}

		if (pos == SOUTHWEST) {
			if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;
				return;
			}
			for (int k = j - 1; k <= j + 1; k++) {
				if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows
						&& inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}
			if (j + 1 < cols && j + 1 >= 0
					&& inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;
				return;
			}
			for (int k = j + 1; k > j - 1; k--) {
				if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0
						&& inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
		}

		if (pos == WEST) {
			for (int k = j - 1; k <= j + 1; k++) {
				if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows
						&& inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}
			if (j + 1 < cols && j + 1 >= 0
					&& inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;
				return;
			}
			for (int k = j + 1; k >= j - 1; k--) {
				if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0
						&& inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
		}

		if (pos == NORTHWEST) {
			if (i - 1 >= 0 && i - 1 < rows
					&& inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}

			for (int r = i - 1; r <= i + 1; r++) {
				if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0
						&& inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;
					return;
				}
			}

			if (i + 1 < rows && i + 1 >= 0
					&& inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
			for (int r = i + 1; r > i - 1; r--) {
				if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols
						&& inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}

		}

		if (pos == NORTH) {
			for (int r = i - 1; r <= i + 1; r++) {
				if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0
						&& inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;
					return;
				}
			}
			if (i + 1 < rows && i + 1 >= 0
					&& inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
			for (int r = i + 1; r >= i - 1; r--) {
				if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols
						&& inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}

		}

		if (pos == NORTHEAST) {
			if (j + 1 < cols && j + 1 >= 0
					&& inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;
				return;
			}
			for (int k = j + 1; k >= j - 1; k--) {
				if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0
						&& inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
			if (j - 1 >= 0 && j - 1 < cols
					&& inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;
				return;
			}
			for (int k = j - 1; k < j + 1; k++) {
				if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows
						&& inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}

		}

		{
			iOut = -1;
			jOut = -1;
			return;
		}
	}
}
void counterClockWiseCheck(ptr_IntMatrix inputImage, int i, int j, int ci,
		int cj, int &iOut, int &jOut) {

	Direction pos = getPosition(i, j, ci, cj);
	int rows = inputImage->getRows();
	int cols = inputImage->getCols();

	if (pos == EAST) {
		for (int k = j + 1; k >= j - 1; k--) {
			if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows) {
				if (inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;

					return;
				}
			}
		}
		if (j - 1 >= 0 && j - 1 < cols && i < rows) {
			if (inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;

				return;
			}
		}
		for (int k = j - 1; k <= j + 1; k++) {
			if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0) {
				if (inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;

					return;
				}
			}
		}
		if (j + 1 < cols && j + 1 >= 0 && i < rows) {
			if (inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;

				return;
			}
		}
	}
	if (pos == SOUTHEAST) {
		if (j + 1 < cols && j + 1 >= 0 && i < rows) {
			if (inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;

				return;
			}

		}
		for (int k = j + 1; k >= j - 1; k--) {
			if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows) {
				if (inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}
		}
		if (j - 1 >= 0 && j - 1 < cols && i < rows) {
			if (inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;
				return;
			}
		}
		for (int k = j - 1; k <= j + 1; k++) {
			if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0) {
				if (inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
		}

	}

	if (pos == SOUTH) {
		for (int r = i + 1; r >= i - 1; r--) {
			if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0) {
				if (inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;

					return;
				}
			}
		}
		if (i - 1 >= 0 && i - 1 < rows && j < cols) {
			if (inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}
		}
		for (int r = i - 1; r <= i + 1; r++) {
			if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols) {
				if (inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}
		}
		if (i + 1 < rows && i + 1 >= 0 && j < cols) {
			if (inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
		}
	}

	if (pos == SOUTHWEST) {
		if (i + 1 < rows && i + 1 >= 0 && j < cols) {
			if (inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
		}
		for (int r = i + 1; r >= i - 1; r--) {
			if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0) {
				if (inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;

					return;
				}
			}
		}
		if (i - 1 >= 0 && i - 1 < rows && j < cols) {
			if (inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}
		}
		for (int r = i - 1; r <= i + 1; r++) {
			if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols) {
				if (inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}
		}
	}

	if (pos == WEST) {
		for (int k = j - 1; k <= j + 1; k++) {
			if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0) {
				if (inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
		}
		if (j + 1 < cols && j + 1 >= 0 && i < rows) {
			if (inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;
				return;
			}
		}
		for (int k = j + 1; k >= j - 1; k--) {
			if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows) {
				if (inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}
		}
		if (j - 1 >= 0 && j - 1 < cols && i < rows) {
			if (inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;
				return;
			}
		}
	}

	if (pos == NORTHWEST) {
		if (j - 1 >= 0 && j - 1 < cols && i < rows) {
			if (inputImage->getAtPosition(i, j - 1) != 0) {
				iOut = i;
				jOut = j - 1;
				return;
			}
		}

		for (int k = j - 1; k <= j + 1; k++) {
			if (k >= 0 && k < cols && i + 1 < rows && i + 1 >= 0) {
				if (inputImage->getAtPosition(i + 1, k) != 0) {
					iOut = i + 1;
					jOut = k;
					return;
				}
			}
		}
		if (j + 1 < cols && j + 1 >= 0 && i < rows) {
			if (inputImage->getAtPosition(i, j + 1) != 0) {
				iOut = i;
				jOut = j + 1;
				return;
			}
		}
		for (int k = j + 1; k >= j - 1; k--) {
			if (k >= 0 && k < cols && i - 1 >= 0 && i - 1 < rows) {
				if (inputImage->getAtPosition(i - 1, k) != 0) {
					iOut = i - 1;
					jOut = k;
					return;
				}
			}
		}

	}

	if (pos == NORTH) {
		for (int r = i - 1; r <= i + 1; r++) {
			if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols) {
				if (inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}
		}
		if (i + 1 < rows && i + 1 >= 0 && j < cols) {
			if (inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
		}
		for (int r = i + 1; r >= i - 1; r--) {
			if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0) {
				if (inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;

					return;
				}
			}
		}
		if (i - 1 >= 0 && i - 1 < rows && j < cols) {
			if (inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}
		}

	}

	if (pos == NORTHEAST) {
		if (i - 1 >= 0 && i - 1 < rows && j < cols) {
			if (inputImage->getAtPosition(i - 1, j) != 0) {
				iOut = i - 1;
				jOut = j;
				return;
			}
		}
		for (int r = i - 1; r <= i + 1; r++) {
			if (r >= 0 && r < rows && j - 1 >= 0 && j - 1 < cols) {
				if (inputImage->getAtPosition(r, j - 1) != 0) {
					iOut = r;
					jOut = j - 1;
					return;
				}
			}
		}
		if (i + 1 < rows && i + 1 >= 0 && j < cols) {
			if (inputImage->getAtPosition(i + 1, j) != 0) {
				iOut = i + 1;
				jOut = j;
				return;
			}
		}
		for (int r = i + 1; r >= i - 1; r--) {
			if (r >= 0 && r < rows && j + 1 < cols && j + 1 >= 0) {
				if (inputImage->getAtPosition(r, j + 1) != 0) {
					iOut = r;
					jOut = j + 1;
					return;
				}
			}
		}

	}

	/*{
	 iOut = -1;
	 jOut = -1;
	 return;
	 }*/

}
vector<ptr_Edge> suzuki(ptr_IntMatrix inputImage) {
	vector<vector<ptr_Point> > edges;
	vector<ptr_Point> edge;
	int rows = inputImage->getRows();
	int cols = inputImage->getCols();
	int nBD = 2;
	int lNBD = 0;
	int i1 = -1, j1 = -1, i2 = -1, j2 = -1, i3 = -1, j3 = -1, i4 = -1, j4 = -1;
	bool flag = false;

	for (int i = 0; i < rows; i++) {
		lNBD = 0;
		for (int j = 0; j < cols; j++) {
			if (((j - 1) >= 0) && inputImage->getAtPosition(i, j) == 255
					&& inputImage->getAtPosition(i, j - 1) == 0 && lNBD <= 0) {
				i2 = i;
				j2 = j - 1;

				// step 3.1
				clockWiseCheck(inputImage, i, j, i2, j2, i1, j1);
				if (i1 == -1 && j1 == -1) {
					inputImage->setAtPosition(i, j, -nBD);
					edge.push_back(new Point(j, i));
					//cout << "\n" << i << "\t" << j << "\n";
					goto step4;
				} else {

					i2 = i1;
					j2 = j1;
					i3 = i;
					j3 = j;

					step33: if (i3 == 2063 && j3 == 1573) {
					 cout << "\nabc";
					 cout << i + j;
					 }
					edge.push_back(new Point(j3, i3));
					counterClockWiseCheck(inputImage, i3, j3, i2, j2, i4, j4);

					if (j3 + 1 < cols
							&& inputImage->getAtPosition(i3, j3 + 1) == 0 && !flag) {
						inputImage->setAtPosition(i3, j3, -nBD);

					} else {
						if (j3 + 1 < cols
								&& inputImage->getAtPosition(i3, j3 + 1) != 0
								&& inputImage->getAtPosition(i3, j3) == 255) {
							inputImage->setAtPosition(i3, j3, nBD);

						}
					}
					if (i4 == i && j4 == j && i3 == i1 && j3 == j1) {
						goto step4;
					} else {

						i2 = i3;
						j2 = j3;
						i3 = i4;
						j3 = j4;
						goto step33;
					}

				}

			}

			step4: if (inputImage->getAtPosition(i, j) != 255) {
				if (edge.size() > 0 && (lNBD == 0 || lNBD == -2)) {
					cout << "\ntete: " << edge.size() << "\t" << lNBD;
					edges.push_back(edge);
					//lNBD = inputImage->getAtPosition(i, j);
					cout<<"\nvalue: "<< inputImage->getAtPosition(1361,964);
				}

				if(inputImage->getAtPosition(i,j) != 0)
					lNBD = inputImage->getAtPosition(i, j);

				edge.clear();
			}

		}

	}

	ptr_IntMatrix output = new Matrix<int>(inputImage->getRows(),
			inputImage->getCols(), 0);
	cout << "\n Size of edges: " << edges.size();
	vector<ptr_Edge> result;

	ofstream of("output/SuzukiValues.txt");
	int countpx = 0;
	for (int i = 0; i < edges.size(); i++) {
		vector<ptr_Point> edge = edges.at(i);
		countpx += edge.size();
		result.push_back(new Edge(edge));
		//if (i == 7 || i == 22 || i == 48 || i == 50 || i == 83) {
			for (int j = 0; j < edge.size(); j++) {
				ptr_Point p = edge.at(j);
				output->setAtPosition(p->getY(), p->getX(), 255);
				of << p->getY() << "\t" << p->getX() << "\n";
			}
		//}
		of << "\n";

	}
	of.close();
	cout << "\nTotal pixels after Suzuki: " << countpx;
	saveGrayScale("output/new_suzuki.jpg", output);
	return result;
}
