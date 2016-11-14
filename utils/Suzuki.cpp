/*
 * Suzuki.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: linh
 */
#include <iostream>
#include <math.h>
#include <stdlib.h>
//#include <stdio.h>
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

enum Direction {
	EAST = 1, WEST = 2, SOUTH = 3, NORTH = 4,
};

// indicate the position of neighbor(ni,nj) with (i,j)
Direction getPosition(int i, int j, int ni, int nj) {
	if (i == ni && j < nj) {
		return EAST;
	}
	if (i == ni && j > nj) {
		return WEST;
	}
	if (j == nj && i < ni) {
		return SOUTH;
	}
	return NORTH;

}

void clockWiseCheck(ptr_IntMatrix inputImage, int i, int j, int ci, int cj,
		int &iOut, int &jOut) {
	Direction pos = getPosition(i, j, ci, cj);

	if (inputImage->getAtPosition(ci, cj) != 0) {
		iOut = ci;
		jOut = cj;
		return;
	} else {
		//int pos = getPosition(i, j, ci, cj);

		if (pos == EAST) {
			//ni = i + 1;
			//nj = j;
			if ((i + 1) < inputImage->getRows()
					&& inputImage->getAtPosition(i + 1, j) == 255) {
				iOut = i + 1;
				jOut = j;
				return;
			} else {
				//ni = i;
				//nj = j - 1;
				if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) == 255) {
					iOut = i;
					jOut = j - 1;
					return;
				} else {
					//ni = i - 1;
					//nj = j;
					if (i - 1 >= 0
							&& inputImage->getAtPosition(i - 1, j) == 255) {
						iOut = i - 1;
						jOut = j;
						return;
					}
				}
			}
		}
		if (pos == NORTH) {
			//ni = i;
			//nj = j + 1;
			if (j + 1 < inputImage->getCols()
					&& inputImage->getAtPosition(i, j + 1) == 255) {
				iOut = i;
				jOut = j + 1;
				return;
			} else {
				//ni = i + 1;
				//nj = j;
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) == 255) {
					iOut = i + 1;
					jOut = j;
					return;
				} else {
					//ni = i;
					//nj = j - 1;
					if (j - 1 >= 0
							&& inputImage->getAtPosition(i, j - 1) == 255) {
						iOut = i;
						jOut = j - 1;
						return;
					}
				}
			}
		}
		if (pos == WEST) {
			//ni = i - 1;
			//nj = j;
			if (i - 1 >= 0 && inputImage->getAtPosition(i - 1, j) == 255) {
				iOut = i - 1;
				jOut = j;
				return;
			} else {
				//ni = i;
				//nj = j + 1;
				if (j + 1 < inputImage->getCols()
						&& inputImage->getAtPosition(i, j + 1) == 255) {
					iOut = i;
					jOut = j + 1;
					return;
				} else {
					//ni = i + 1;
					//nj = j;
					if (i + 1 < inputImage->getRows()
							&& inputImage->getAtPosition(i + 1, j) == 255) {
						iOut = i + 1;
						jOut = j;
						return;
					}
				}
			}
		}
		if (pos == SOUTH) {
			//ni = i;
			//nj = j - 1;
			if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) == 255) {
				iOut = i;
				jOut = j - 1;
				return;
			} else {
				//ni = i - 1;
				//nj = j;
				if (i - 1 >= 0 && inputImage->getAtPosition(i - 1, j) == 255) {
					iOut = i - 1;
					jOut = j;
					return;
				} else {
					//ni = i;
					//nj = j + 1;
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) == 255) {
						iOut = i;
						jOut = j + 1;
						return;
					}
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
/*ptr_Point counterClockWiseCheck(ptr_IntMatrix inputImage, int i, int j, int ci,
		int cj, bool& flag) {

	flag = false; // check i, j + 1 in same hole with i, j -1
	Direction pos = getPosition(i, j, ci, cj);
	ptr_Point p = new Point(-1, -1);
	if (pos == EAST) {
		//ni = i - 1;
		//nj = j;
		if (i - 1 >= 0 && inputImage->getAtPosition(i - 1, j) == 255) {
			p->setX(j);
			p->setY(i - 1);
			return p;
		} else {
			//ni = i;
			//nj = j - 1;
			if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) == 255) {
				p->setX(j - 1);
				p->setY(i);
				return p;
			} else {
				//ni = i + 1;
				//nj = j;
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) == 255) {
					p->setX(j);
					p->setY(i + 1);
					return p;
				} else {

				 if (inputImage->getAtPosition(ci, cj) != 0) {
				 p->setX(cj);
				 p->setY(ci);
				 flag = true;
				 return p;
				 }
				 }
			}
		}
	} else {
		if (pos == NORTH) {
			//ni = i;
			//nj = j - 1;
			if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) == 255) {

				p->setX(j - 1);
				p->setY(i);
				return p;
			} else {
				//ni = i + 1;
				//nj = j;
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) == 255) {

					p->setX(j);
					p->setY(i + 1);
					return p;
				} else {
					//ni = i;
					//nj = j + 1;
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) == 255) {
						p->setX(j + 1);
						p->setY(i);
						return p;
					} else {

					 if (inputImage->getAtPosition(ci, cj) != 0) {

					 p->setX(cj);
					 p->setY(ci);
					 flag = true;
					 return p;
					 }
					 }
				}
			}
		} else {
			if (pos == WEST) {
				//ni = i + 1;
				//nj = j;
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) == 255) {

					p->setX(j);
					p->setY(i + 1);
					return p;
				} else {
					//ni = i;
					//nj = j + 1;
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) == 255) {

						p->setX(j + 1);
						p->setY(i);
						return p;
					} else {
						//ni = i - 1;
						//nj = j;
						if (i - 1 < inputImage->getRows()
								&& inputImage->getAtPosition(i - 1, j) == 255) {

							p->setX(j);
							p->setY(i - 1);
							return p;
						} else {
						 if (inputImage->getAtPosition(ci, cj) != 0) {

						 p->setX(cj);
						 p->setY(ci);
						 flag = true;
						 return p;
						 }

						 }
					}
				}
			} else {
				if (pos == SOUTH) {
					//ni = i;
					//nj = j + 1;
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) == 255) {

						p->setX(j + 1);
						p->setY(i);
						return p;
					} else {
						//ni = i - 1;
						//nj = j;
						if (i - 1 >= 0
								&& inputImage->getAtPosition(i - 1, j) == 255) {

							p->setX(j);
							p->setY(i - 1);
							return p;
						} else {
							//ni = i;
							//nj = j - 1;
							if (j - 1 >= 0
									&& inputImage->getAtPosition(i, j - 1)
											== 255) {
								p->setX(j - 1);
								p->setY(i);
								return p;
							} else {
							 if (inputImage->getAtPosition(ci, cj) != 0) {

							 p->setX(cj);
							 p->setY(ci);
							 flag = true;
							 return p;
							 }

							 }
						}
					}
				}
			}
		}
	}
	flag = true;
	p->setX(-1);
	p->setY(-1);
	return p;

}*/

ptr_Point counterClockWiseCheck(ptr_IntMatrix inputImage, int i, int j, int ci,
		int cj, bool& flag) {

	flag = false; // check i, j + 1 in same hole with i, j -1
	Direction pos = getPosition(i, j, ci, cj);
	ptr_Point p = new Point(-1, -1);
	if (pos == EAST) {
		if (i - 1 >= 0 && inputImage->getAtPosition(i - 1, j) != 0) {
			if (inputImage->getAtPosition(i - 1, j) == 255) {
				p->setX(j);
				p->setY(i - 1);
			} else {
				p->setX(-1);
				p->setY(-1);
			}
			return p;
		} else {
			if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) != 0) {
				if (inputImage->getAtPosition(i, j - 1) == 255) {
					p->setX(j - 1);
					p->setY(i);
				} else {
					p->setX(-1);
					p->setY(-1);
				}
				return p;
			} else {
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) != 0) {
					if (inputImage->getAtPosition(i + 1, j) == 255) {
						p->setX(j);
						p->setY(i + 1);
					} else {
						p->setX(-1);
						p->setY(-1);
					}
					return p;
				}
			}
		}
	} else {
		if (pos == NORTH) {
			if (j - 1 >= 0 && inputImage->getAtPosition(i, j - 1) != 0) {
				if (inputImage->getAtPosition(i, j - 1) == 255) {
					p->setX(j - 1);
					p->setY(i);
				} else {
					p->setX(-1);
					p->setY(-1);
				}
				return p;
			} else {
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) != 0) {
					if (inputImage->getAtPosition(i + 1, j) == 255) {
						p->setX(j);
						p->setY(i + 1);
					} else {
						p->setX(-1);
						p->setY(-1);
					}
					return p;
				} else {
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) != 0) {
						if (inputImage->getAtPosition(i, j + 1) == 255) {
							p->setX(j + 1);
							p->setY(i);
						} else {
							p->setX(-1);
							p->setY(-1);
						}
						return p;
					}
				}
			}
		} else {
			if (pos == WEST) {
				if (i + 1 < inputImage->getRows()
						&& inputImage->getAtPosition(i + 1, j) != 0) {
					if (inputImage->getAtPosition(i + 1, j) == 255) {
						p->setX(j);
						p->setY(i + 1);
					} else {
						p->setX(-1);
						p->setY(-1);
					}
					return p;
				} else {
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) != 0) {
						if (inputImage->getAtPosition(i, j + 1) == 255) {
							p->setX(j + 1);
							p->setY(i);
						} else {
							p->setX(-1);
							p->setY(-1);
						}
						return p;
					} else {
						flag = true;
						if (i - 1 < inputImage->getRows()
								&& inputImage->getAtPosition(i - 1, j) != 0) {
							if (inputImage->getAtPosition(i - 1, j) == 255) {
								p->setX(j);
								p->setY(i - 1);
							} else {
								p->setX(-1);
								p->setY(-1);
							}
							return p;
						}
					}
				}
			} else {
				if (pos == SOUTH) {
					if (j + 1 < inputImage->getCols()
							&& inputImage->getAtPosition(i, j + 1) != 0) {
						if (inputImage->getAtPosition(i, j + 1) == 255) {
							p->setX(j + 1);
							p->setY(i);
						} else {
							p->setX(-1);
							p->setY(-1);
						}
						return p;
					} else {
						flag = true;
						if (i - 1 >= 0
								&& inputImage->getAtPosition(i - 1, j) != 0) {
							if (inputImage->getAtPosition(i - 1, j) == 255) {
								p->setX(j);
								p->setY(i - 1);
							} else {
								p->setX(-1);
								p->setY(-1);
							}
							return p;
						} else {
							if (j - 1 >= 0
									&& inputImage->getAtPosition(i, j - 1)
											!= 0) {
								if (inputImage->getAtPosition(i, j - 1)
										== 255) {
									p->setX(j - 1);
									p->setY(i);
								} else {
									p->setX(-1);
									p->setY(-1);
								}
								return p;
							}
						}
					}
				}
			}
		}
	}
	flag = true;
	p->setX(-1);
	p->setY(-1);
	return p;

}
void suzuki(ptr_IntMatrix inputImage) {
	vector<vector<ptr_Point> > edges;
	vector<ptr_Point> edge;
	int rows = inputImage->getRows();
	int cols = inputImage->getCols();
	int nBD = 2;
	int lNBD = 0;
	int i1 = -1, j1 = -1, i2 = -1, j2 = -1, i3 = -1, j3 = -1, i4 = -1, j4 = -1;
	bool flag = false;

	//cout << "\n Value at 2, 128: " << inputImage->getAtPosition(2, 128);
	for (int i = 0; i < rows; i++) {
		lNBD = 0;
		for (int j = 0; j < cols; j++) {
			if (((j - 1) >= 0) && inputImage->getAtPosition(i, j) == 255
					&& inputImage->getAtPosition(i, j - 1) == 0 && lNBD <= 0) {
				i2 = i;
				j2 = j - 1;
				//if(lNBD ==0 || lNBD == 3)
				//	edge.push_back(new Point(j, i));
				//cout << "\n i: " << i << "\t" << j;
				// step 3.1
				clockWiseCheck(inputImage, i, j, i2, j2, i1, j1);
				if (i1 == -1 && j1 == -1) {
					inputImage->setAtPosition(i, j, -nBD);

					goto step4;
				} else {
					//cout << "\n i1: " << i1 << "\t" << j1;
					i2 = i1;
					j2 = j1;
					i3 = i;
					j3 = j;

					step33: ptr_Point p = counterClockWiseCheck(inputImage, i3,
							j3, i2, j2, flag);
					i4 = p->getY();
					j4 = p->getX();
					if(i4 != -1 && j4 != -1){
						if((inputImage->getAtPosition(i4,j4-1) != 0 && inputImage->getAtPosition(i4,j4-1) != 255)
								|| (inputImage->getAtPosition(i4,j4-2) != 0 && inputImage->getAtPosition(i4,j4-2) != 255))
							goto step4;
					}

					if (inputImage->getAtPosition(i3, j3 + 1) == 0 ) {
						inputImage->setAtPosition(i3, j3, 3);

					} else {
						if (inputImage->getAtPosition(i3, j3 + 1) != 0
								&& inputImage->getAtPosition(i3, j3) == 255) {
							inputImage->setAtPosition(i3, j3, nBD);

						}
					}
					if ((i4 == -1 && j4 == -1)
							|| (i4 == i && j4 == i && i3 == i1 && j3 == j1))
						goto step4;
					else {
						//if(lNBD == 0 || lNBD == 3)
							edge.push_back(new Point(j3, i3));
						i2 = i3;
						j2 = j3;
						i3 = i4;
						j3 = j4;
						goto step33;
					}

				}

			}

			step4: if (inputImage->getAtPosition(i, j) != 255) {
				lNBD = abs(inputImage->getAtPosition(i, j));

				if (edge.size() > 1) {
					edges.push_back(edge);
				}
				edge.clear();
			}

		}


	}

	ptr_IntMatrix output = new Matrix<int>(inputImage->getRows(),
			inputImage->getCols(), 0);
	cout << "\n Size of edges: " << edges.size();
	ofstream of("output/edgeValues.txt");
	int countpx = 0;
	for (int i = 0; i < edges.size(); i++) {
		vector<ptr_Point> edge = edges.at(i);
		for (int j = 0; j < edge.size(); j++) {
			ptr_Point p = edge.at(j);
			output->setAtPosition(p->getY(), p->getX(), 255);
			of << p->getY() << "\t" << p->getX() << "\n";
			countpx++;
		}
		of << "\n";
	}
	of.close();
	cout<<"\nTotal pixels after Suzuki: "<<countpx;
	/*saveGrayJPG(output, output->getCols(), output->getRows(),
			"output/new_canny2222.jpg");*/
}
