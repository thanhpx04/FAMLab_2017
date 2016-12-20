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
vector<ptr_Point> drawingLine(ptr_Line line, RGB color)
{
	ptr_Point begin = line->getBegin();
	ptr_Point end = line->getEnd();
	begin->setColor(color);
	end->setColor(color);
	int dx = end->getX() - begin->getX();
	int dy = end->getY() - begin->getY();
	vector<ptr_Point> listOfPoints;
	int x = begin->getX();
	int y = begin->getY();
	listOfPoints.push_back(new Point(x, y, color));
	if (dy < dx)
	{
		float p = dy - dx / 2;
		while (x < end->getX())
		{
			if (p >= 0)
			{
				listOfPoints.push_back(new Point(x + 1, y + 1, color));
				y++;
				p += dy - dx;
			}
			else
			{
				listOfPoints.push_back(new Point(x + 1, y, color));
				p += dy;
			}
			x++;
		}
	}
	else
	{
		float p = dx / 2 - y;
		while (y <= end->getY())
		{
			if (p >= 0)
			{
				listOfPoints.push_back(new Point(x + 1, y + 1, color));
				x++;
				p += dx - dy;
			}
			else
			{
				listOfPoints.push_back(new Point(x, y + 1, color));
				p += dx;
			}
			y++;
		}
	}
	listOfPoints.push_back(new Point(end->getX(), end->getY(), color));
	return listOfPoints;
}

vector<ptr_Point> put8pixel(ptr_Point center, ptr_Point p, RGB color)
{
	int xc = center->getX();
	int yc = center->getY();
	int x = p->getX();
	int y = p->getY();
	vector<ptr_Point> eightPoints;
	eightPoints.push_back(new Point(x + xc, y + yc, color));
	eightPoints.push_back(new Point(-x + xc, y + yc, color));
	eightPoints.push_back(new Point(x + xc, -y + yc, color));
	eightPoints.push_back(new Point(-x + xc, -y + yc, color));
	eightPoints.push_back(new Point( y + xc, x + yc, color));
	eightPoints.push_back(new Point(-y + xc, x + yc, color));
	eightPoints.push_back(new Point(y + xc, -x + yc, color));
	eightPoints.push_back(new Point(-y + xc, -x + yc, color));
	return eightPoints;
}
vector<ptr_Point> drawingCircle(ptr_Point center, int radius, RGB color)
{
	int x = 0;
	int y = radius;
	int f = 1 - radius;
	vector<ptr_Point> result;
	vector<ptr_Point> drawPoints = put8pixel(center,new Point(x,y),color);
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
		drawPoints = put8pixel(center,new Point(x,y),color);
		result.insert(result.end(),drawPoints.begin(),drawPoints.end());
	}
	return result;
}
