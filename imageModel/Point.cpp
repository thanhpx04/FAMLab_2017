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
	color.R = color.G = color.B = 0;
}

Point::~Point()
{
	// TODO Auto-generated destructor stub
}

RGB Point::getColor()
{
	return color;
}
void Point::setColor(RGB rgb)
{
	color.R = rgb.R;
	color.G = rgb.G;
	color.B = rgb.B;
}
