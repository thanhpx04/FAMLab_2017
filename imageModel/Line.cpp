/*
 * Line.cpp
 *
 *  Created on: Sep 14, 2016
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

#include "Point.h"
#include "Line.h"

//=================================================== Constructor ===========================================
Line::Line()
{}
Line::Line(const Line& otherLine)
{
	begin = otherLine.begin;
	end = otherLine.end;
	dx = otherLine.dx;
	dy = otherLine.dy;
	length = otherLine.length;
	equation = otherLine.equation;
}

Line::Line(ptr_Point ep1, ptr_Point ep2)
{
    begin = ep1;
    end = ep2;

    dx = end->getX() - begin->getX();
    dy = end->getY() - begin->getY();

    length = lengthOfLine();

    equation = equationOfLine();
}

Line::~Line()
{
    // TODO Auto-generated destructor stub
}
//=================================================== Get and set ===========================================
ptr_Point Line::getBegin()
{
    return begin;
}
ptr_Point Line::getEnd()
{
    return end;
}

void Line::setBegin(ptr_Point ep1)
{
    begin = ep1;
}
void Line::setEnd(ptr_Point ep2)
{
    end = ep2;
}

double Line::getLength()
{
    return length;
}
std::vector<double> Line::getEquation()
{
    return equation;
}

//=================================================== Private methods ===========================================
/*
 * Check the line is a point or not
 */
bool Line::isPoint()
{
    if (begin->getX() == end->getX() && begin->getY() == end->getY())
		return true;
    return false;
}

/*
 * Calculate the length of line
 */
double Line::lengthOfLine()
{
    return sqrt(pow((double)dx, 2) + pow((double)dy, 2));
}

/*
 * Get the equation of line (ax + by + c = 0)
 */
std::vector<double> Line::equationOfLine()
{
    std::vector<double> equa;
    if (dy == 0)
    { // line y = m
		equa.push_back(0);
		equa.push_back(1);
		equa.push_back(-(begin->getY()));
		return equa;
    }

    if (dx == 0)
    { //line x = n
		equa.push_back(1);
		equa.push_back(0);
		equa.push_back(-(begin->getX()));
		return equa;
    }

    if (dx != 0 && dy != 0)
    { // normal line
		double m = (double)dy / (double)dx;

		equa.push_back(m);
		equa.push_back(-1);
		equa.push_back(begin->getY() - (m * begin->getX()));
		return equa;
    }
    return equa; // NULL
}
//=================================================== Public Methods ===========================================
double Line::perpendicularDistance(ptr_Point point)
{
    double a = equation.at(0);
    double b = equation.at(1);
    double c = equation.at(2);

    if (a == 0 && b == 1)
    {				       // y = c
	return abs(point->getY() + c); //because the eqution of line is y - c = 0
    }
    if (a == 1 && b == 0)
    { // x-c = 0
	return abs(point->getX() + c);
    }
    return abs((a * ((double)point->getX())) + (b * ((double)point->getY())) + c) / (sqrt(pow((double)a, 2) + pow((double)b, 2)));
}

double Line::angleLines(Line otherLine)
{

    double slope1, slope2;
    if (dx == 0 && dy == 0)
	return 0;
    if (dx == 0 && otherLine.dx != 0)
    { // line 1 is parallel with Oy
	slope2 = (double)otherLine.dy / (double)otherLine.dx;
	return atan(abs(1 / slope2)) * 180 / M_PI;
    }
    if (dx != 0 && otherLine.dx == 0)
    { //otherLine is parallel with Oy
	slope1 = (double)dy / (double)dx;
	return atan(abs(1 / slope1)) * 180 / M_PI;
    }

    slope1 = (double)dy / (double)dx;
    slope2 = (double)otherLine.dy / (double)otherLine.dx;

    if (slope1 == slope2) // parallel lines
	return 0;
    if (slope1 * slope2 == -1) // perpendicular lines
	return 90;
    if (slope1 == 0 && slope2 != 0)
    { // line 1 is parallel with Ox
	return atan(abs(slope2)) * 180 / M_PI;
    }
    if (slope1 != 0 && slope2 == 0)
    { // otherLine is parallel with Ox
	return atan(abs(slope1)) * 180 / M_PI;
    }
    return atan(abs((slope1 - slope2) / (1 + slope1 * slope2))) * 180 / M_PI;
}

ptr_Point Line::intersection(Line otherLine)
{

    std::vector<double> otherEquation = otherLine.equation;

    if (equation.at(0) == 0 && otherEquation.at(0) != 0)
    {
		return new Point((otherEquation.at(1) * equation.at(2) - otherEquation.at(2) * equation.at(1)) / (otherEquation.at(0) * equation.at(1)), -equation.at(2) / equation.at(1));
    }
    if (equation.at(0) != 0 && otherEquation.at(0) == 0)
    {
		return new Point((equation.at(1) * otherEquation.at(2) - (equation.at(2) * otherEquation.at(1))) / (equation.at(0) * otherEquation.at(1)), -otherEquation.at(2) / otherEquation.at(1));
    }
    if (equation.at(1) == 0 && otherEquation.at(1) != 0)
    {
		return new Point(-equation.at(2), otherEquation.at(0) * equation.at(2) - otherEquation.at(2));
    }
    if (equation.at(1) != 0 && otherEquation.at(1) == 0)
    {
		return new Point(-otherEquation.at(2), (equation.at(0) * otherEquation.at(2) - equation.at(2)) / equation.at(1));
    }
    if ((equation.at(0) == otherEquation.at(0)) || (equation.at(1) == otherEquation.at(1) && equation.at(1) == 0))
    { // parallel lines
		return new Point(-1, -1);
    }
    double da = equation.at(0) - otherEquation.at(0);
    double dc = otherEquation.at(2) - equation.at(2);
    return new Point(round(dc / da), round((equation.at(0) * (dc / da)) + equation.at(2)));
}

bool Line::checkBelongPoint(ptr_Point point)
{
    if (perpendicularDistance(point) == 0)
	return true;
    return false;
}
