/*
 * Line.h
 *
 *  Created on: Sep 14, 2016
 *      Author: linh
 */

#ifndef LINE_H_
#define LINE_H_

class Line
{

  private:
    ptr_Point begin;
    ptr_Point end;
    int dx; // slope of x
    int dy; // slope of y
    double length;
    std::vector<double> equation;

    bool isPoint();
    double lengthOfLine();
    std::vector<double> equationOfLine();

  public:
    Line();
    Line(const Line&);
    Line(ptr_Point, ptr_Point);
    virtual ~Line();
    ptr_Point getBegin();
    ptr_Point getEnd();
    double getLength();
    std::vector<double> getEquation();
    void setBegin(ptr_Point);
    void setEnd(ptr_Point);
    double perpendicularDistance(ptr_Point);
    double angleLines(Line);
    ptr_Point intersection(Line);
    bool checkBelongPoint(ptr_Point);
};
typedef Line* ptr_Line;
#endif /* LINE_H_ */
