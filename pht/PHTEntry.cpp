/*
 * PHTEntry.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"

#include "PHTEntry.h"

//============================== Method of class ==========================
PHTEntry::PHTEntry()
{
	// TODO Auto-generated constructor stub

}

PHTEntry::~PHTEntry()
{
	// TODO Auto-generated destructor stub
}
vector<HoughSpace> PHTEntry::getListHoughSpace()
{
	return listHoughSpace;
}

void PHTEntry::setListHoughSpace(vector<HoughSpace> listHoughSpace)
{
	this->listHoughSpace = listHoughSpace;
}

ptr_Line PHTEntry::getObjLine()
{
	return oLine;
}

void PHTEntry::setObjLine(ptr_Line line)
{
	oLine = line;
}

ptr_Line PHTEntry::getRefLine()
{
	return rLine;
}

void PHTEntry::setRefLine(ptr_Line line)
{
	rLine = line;
}
void PHTEntry::addHoughSpace(HoughSpace hSpace)
{
	listHoughSpace.push_back(hSpace);
}
