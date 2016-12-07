/*
 * Point.h
 *
 *  Created on: Sep 14, 2016
 *      Author: linh
 */

#ifndef POINT_H_
#define POINT_H_

class Point
{
  private:
    int x;
    int y;

  public:
    Point();
    Point(int xCoor, int yCoor){x = xCoor; y = yCoor;}
    virtual ~Point();
    int getX(){return x;}
    void setX(int xCoor){x = xCoor;}
    int getY(){return y;}
    void setY(int yCoor) {y = yCoor;}
    Point operator+(Point p);
};

typedef Point *ptr_Point;

#endif /* POINT_H_ */
