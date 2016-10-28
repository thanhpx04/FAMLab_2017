/*
 * Edge.h
 *
 *  Created on: Sep 15, 2016
 *      Author: linh
 */

#ifndef EDGE_H_
#define EDGE_H_



class Edge {

private:
	std::vector<ptr_Point> listOfPoints; // list of points in the edge
	std::vector<ptr_Point> listOfBreakPoints; // list of break points in edge. Break point is connection point between two lines on edge.

	bool checkPointInList(std::vector<ptr_Point>,ptr_Point);
	std::vector<ptr_Line> getLines(std::vector<ptr_Point>);
	void breakEdge(int);

public:
	Edge();
	virtual ~Edge();
	Edge(std::vector<ptr_Point>);

	std::vector<ptr_Point> getPoints();
	void setPoints(std::vector<ptr_Point>);
	std::vector<ptr_Line> segment(int);
	bool isWeakEdge();

};
typedef Edge* ptr_Edge;
#endif /* EDGE_H_ */
