/*
 * Point.h
 *
 *  Created on: Sep 14, 2016
 *      Author: linh
 */

#ifndef POINT_H_
#define POINT_H_
struct RGB {
	unsigned short int R;
	unsigned short int G;
	unsigned short int B;
};

class Point
{
  private:
    int x;
    int y;
    RGB color;
  public:
    Point();
    Point(int xCoor, int yCoor){x = xCoor; y = yCoor; color.R = color.G = color.B = 0;}
    Point(int xCoor, int yCoor,RGB clor){x = xCoor; y = yCoor; color.R =clor.R; color.G = clor.G; color.B = clor.B;}
    virtual ~Point();
    int getX(){return x;}
    void setX(int xCoor){x = xCoor;}
    int getY(){return y;}
    void setY(int yCoor) {y = yCoor;}
    RGB getColor();
    void setColor(RGB rgb);
    void toString();
};

typedef Point *ptr_Point;

#endif /* POINT_H_ */
