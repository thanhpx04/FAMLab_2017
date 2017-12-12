/*
 * Point.h
 *
 *  Created on: Sep 14, 2016
 *      Author: linh
 */

#ifndef POINT_H_
#define POINT_H_

#include <iostream>
using namespace std;

struct RGB
{
	unsigned long int R;
	unsigned long int G;
	unsigned long int B;
	RGB operator+(const RGB& color) const
	{
		RGB result;
		result.R = color.R + R;
		result.G = color.G + G;
		result.B = color.B + B;
		return result;
	}
	RGB operator*(const RGB& color) const
	{
		RGB result;
		result.R = color.R * R;
		result.G = color.G * G;
		result.B = color.B * B;
		return result;
	}
	RGB operator/(const double c) const
	{
		RGB result;
		result.R = R / c;
		result.G = G / c;
		result.B = B / c;
		return result;
	}
	RGB operator+=(const RGB& color) const
	{
		RGB result;
		result.R = color.R + R;
		result.G = color.G + G;
		result.B = color.B + B;
		return result;
	}
	bool operator==(const unsigned long int i) const
	{
		if (R == i && G == i && B == i)
			return true;
		return false;
	}
	bool operator!=(const unsigned long int i) const
	{
		if (R != i && G != i && B != i)
			return true;
		return false;
	}
	bool operator>(const unsigned long int i) const
	{
		if (R > i && G > i && B > i)
			return true;
		return false;
	}
	bool operator>=(const unsigned long int i) const
	{
		if (R >= i && G >= i && B >= i)
			return true;
		return false;
	}
	RGB operator=(const unsigned long int value) const
	{
		RGB color;
		color.R = color.G = color.B = value;
		return color;
	}
};

//========= Thanh == structure for PNG image ==================
struct RGBA
{
    unsigned long int R;
    unsigned long int G;
    unsigned long int B;
    unsigned long int A;
    RGBA operator+(const RGBA& color) const
    {
        RGBA result;
        result.R = color.R + R;
        result.G = color.G + G;
        result.B = color.B + B;
        result.A = color.A + A;
        return result;
    }
    RGBA operator*(const RGBA& color) const
    {
        RGBA result;
        result.R = color.R * R;
        result.G = color.G * G;
        result.B = color.B * B;
        result.A = color.A * A;
        return result;
    }
    RGBA operator/(const double c) const
    {
        RGBA result;
        result.R = R / c;
        result.G = G / c;
        result.B = B / c;
        result.A = A / c;
        return result;
    }
    RGBA operator+=(const RGBA& color) const
    {
        RGBA result;
        result.R = color.R + R;
        result.G = color.G + G;
        result.B = color.B + B;
        result.A = color.A + A;
        return result;
    }
    bool operator==(const unsigned long int i) const
    {
        if (R == i && G == i && B == i && A == i)
            return true;
        return false;
    }
    bool operator!=(const unsigned long int i) const
    {
        if (R != i && G != i && B != i && A != i)
            return true;
        return false;
    }
    bool operator>(const unsigned long int i) const
    {
        if (R > i && G > i && B > i && A > i)
            return true;
        return false;
    }
    bool operator>=(const unsigned long int i) const
    {
        if (R >= i && G >= i && B >= i && A >= i)
            return true;
        return false;
    }
    RGBA operator=(const unsigned long int value) const
    {
        RGBA color;
        color.R = color.G = color.B = color.A = value;
        return color;
    }
};
//=================================================

class Point
{
private:
	int x;
	int y;
	RGB color;
public:
	Point();
	Point(const Point &cpPoint);
	Point(int xCoor, int yCoor)
	{
		x = xCoor;
		y = yCoor;
		color.R = color.G = color.B = 0;
	}
	Point(int xCoor, int yCoor, RGB clor)
	{
		x = xCoor;
		y = yCoor;
		color.R = clor.R;
		color.G = clor.G;
		color.B = clor.B;
	}
	virtual ~Point();
	int getX()
	{
		return x;
	}
	void setX(int xCoor)
	{
		x = xCoor;
	}
	int getY()
	{
		return y;
	}
	void setY(int yCoor)
	{
		y = yCoor;
	}
	RGB getColor();
	void setColor(RGB rgb);
	void toString();
	void reset();
	bool operator<(const Point &cPoint);
	bool operator>(const Point &cPoint);
	bool operator==(const Point &cPoint);
	bool operator!=(int value);
	Point operator-(const Point cPoint);
	Point operator+(const Point cPoint);
};

typedef Point *ptr_Point;

#endif /* POINT_H_ */
