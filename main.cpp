/*
 *
 *
 * Test file
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

#include "imageModel/Point.h"
#include "imageModel/Line.h"
#include "imageModel/Edge.h"
#include "imageModel/Matrix.h"
//#include "imageModel/Image.h"
std::vector<ptr_Point> readTPS(const char* filename);

int main()
{
    cout << "MAELab test" << endl
	 << endl;

    // Test Line.h ===========================================================================================

    ptr_Point p1 = new Point(10, 20);
    //Point p1(5,3);
    cout << "x Coordinate: " << p1->getX() << endl;
    cout << "y Coordinate: " << p1->getY() << endl;

    Line l1(new Point(1,1),new Point(9,6));

	cout <<"Length of line 1: " <<l1.getLength()<<endl;

	vector<double> eq = l1.getEquation();
	cout<< "Value of equation: "<< eq.at(0)<<"\t"<<eq.at(1)<<"\t"<<eq.at(2)<<endl;

	cout<< "Distance from p1 to l1:" << l1.perpendicularDistance(p1)<<endl;

	Line l2(new Point(29,5),new Point(32,32));
	vector<double> eq2 = l2.getEquation();
	cout<< "Value of equation line 2: "<< eq2.at(0)<<"\t"<<eq2.at(1)<<"\t"<<eq2.at(2)<<endl;
	ptr_Point p = l1.intersection(l2);
	cout<<"Intersection point: "<< p->getX()<<", "<<p->getY()<<endl;
	cout<<"The angle between two lines: "<< l1.angleLines(l2)<<endl;
	cout<<l1.checkBelongPoint(p)<<endl;

	// Test Edge.h ===============================================================================================
	ptr_Point ep1 = new Point(4,5);
	ptr_Point ep2 = new Point(8,12);
	ptr_Point ep3 = new Point(19,17);
	ptr_Point ep4 = new Point(28,16);
	ptr_Point ep5 = new Point(36,8);
	//ptr_Point ep6 = new Point(12,5);
	//ptr_Point ep7 = new Point(14,2);
	//ptr_Point ep8 = new Point(17,3);

	vector<ptr_Point> listPoints;
	listPoints.push_back(ep1);
	listPoints.push_back(ep2);
	listPoints.push_back(ep3);
	listPoints.push_back(ep4);
	listPoints.push_back(ep5);
	//listPoints.push_back(ep6);
	//listPoints.push_back(ep7);
	//listPoints.push_back(ep8);

	ptr_Edge edge = new Edge(listPoints);
	vector<ptr_Point> bPoints = edge->segment(2.8);
	cout<<"Number of break points: "<<bPoints.size()<<endl;
	for (size_t i = 0; i < bPoints.size(); i++) {
		ptr_Point p = bPoints.at(i);
		cout<<"("<<p->getX()<<", " << p->getY()<<") \t";
	}
	cout<<endl;
	//Test Matrix.h ======================================================================================
	ptr_Matrix inmatrix = new Matrix<int>(3,3,1);
	inmatrix->printMatrix();

	ptr_Matrix otherMT = inmatrix;	
	otherMT->setAtPosition(1,1,10);
	otherMT->printMatrix();

	// matrix of points
	//ptr_PointMatrix points(2,2,new Point(1,1));

	cout<<endl<<"Number: "<<readTPS("/home/linh/Desktop/Temps/mg/Mg 159.TPS").size();

    cout << endl;

    return 0;
}
