/*
 * Drawing.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"

#include "Drawing.h"

// draw a line using MidPoint algorithm
vector<Point> drawingLine(Line line, RGB color)
{
	Point begin = line.getBegin();
	Point end = line.getEnd();
	begin.setColor(color);
	end.setColor(color);
	int dx = end.getX() - begin.getX();
	int dy = end.getY() - begin.getY();
	vector<Point> listOfPoints;
	int x = begin.getX();
	int y = begin.getY();
	listOfPoints.push_back(Point(x, y, color));
	if (dy < dx)
	{
		float p = dy - dx / 2;
		while (x < end.getX())
		{
			if (p >= 0)
			{
				listOfPoints.push_back(Point(x + 1, y + 1, color));
				y++;
				p += dy - dx;
			}
			else
			{
				listOfPoints.push_back(Point(x + 1, y, color));
				p += dy;
			}
			x++;
		}
	}
	else
	{
		float p = dx / 2 - y;
		while (y <= end.getY())
		{
			if (p >= 0)
			{
				listOfPoints.push_back(Point(x + 1, y + 1, color));
				x++;
				p += dx - dy;
			}
			else
			{
				listOfPoints.push_back(Point(x, y + 1, color));
				p += dx;
			}
			y++;
		}
	}
	listOfPoints.push_back(Point(end.getX(), end.getY(), color));
	return listOfPoints;
}

vector<Point> put8pixel(Point center, Point p, RGB color)
{
	int xc = center.getX();
	int yc = center.getY();
	int x = p.getX();
	int y = p.getY();
	vector<Point> eightPoints;
	eightPoints.push_back(Point(x + xc, y + yc, color));
	eightPoints.push_back(Point(-x + xc, y + yc, color));
	eightPoints.push_back(Point(x + xc, -y + yc, color));
	eightPoints.push_back(Point(-x + xc, -y + yc, color));
	eightPoints.push_back(Point( y + xc, x + yc, color));
	eightPoints.push_back(Point(-y + xc, x + yc, color));
	eightPoints.push_back(Point(y + xc, -x + yc, color));
	eightPoints.push_back(Point(-y + xc, -x + yc, color));
	return eightPoints;
}
vector<Point> drawingCircle(Point center, int radius, RGB color)
{
	int x = 0;
	int y = radius;
	int f = 1 - radius;
	vector<Point> result;
	vector<Point> drawPoints = put8pixel(center,Point(x,y),color);
	result.insert(result.end(),drawPoints.begin(),drawPoints.end());
	while(x < y)
	{
		if(f < 0)
		{
			f += 2*x + 3;
		}
		else
		{
			f += 2 * (x-y) + 5;
			y--;
		}
		x++;
		drawPoints = put8pixel(center,Point(x,y),color);
		result.insert(result.end(),drawPoints.begin(),drawPoints.end());
	}
	return result;
}
