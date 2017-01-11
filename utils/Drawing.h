/*
 * Drawing.h
 *
 *  Created on: Dec 18, 2016
 *      Author: linh
 */

#ifndef DRAWING_H_
#define DRAWING_H_

vector<Point> drawingLine(Line line, RGB color);
vector<Point> drawingCircle(Point center,int radius, RGB color);
vector<Point> fillCircle(Point center, int radius, RGB color) ;
#endif /* DRAWING_H_ */
