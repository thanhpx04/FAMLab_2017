/*
 * Point.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: linh
 */

#include "Point.h"

/*
 * Constructor the Point
 */
Point::Point()
{
    x = 0;
    y = 0;
}

Point::~Point()
{
    // TODO Auto-generated destructor stub
}
Point Point::operator+(Point p)
{
	Point q;
	q.x += p.x;
	q.y += p.y;
	return q;
}
