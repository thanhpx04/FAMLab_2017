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
	std::vector<ptr_Line> listOfLines; // list of approximated lines of edge
	std::vector<ptr_Point> listOfPoints; // list of points in the edge
	std::vector<ptr_Point> listOfBreakPoints; // list of break points in edge. Break point is connection point between two lines on edge.

	bool checkPointInList(std::vector<ptr_Point>,ptr_Point);
	void addLine(ptr_Line);
	std::vector<ptr_Line> getLines(std::vector<ptr_Point>);
	std::vector<ptr_Point> getPoints(std::vector<ptr_Line>);
	void breakEdge(int);

public:
	Edge();
	virtual ~Edge();
	Edge(std::vector<ptr_Line>);
	Edge(std::vector<ptr_Point>);
	std::vector<ptr_Line> getLines();
	std::vector<ptr_Point> getPoints();
	void setLines(std::vector<ptr_Line>);
	void setPoints(std::vector<ptr_Point>);
	std::vector<ptr_Point> segment(int);

};
typedef Edge* ptr_Edge;
#endif /* EDGE_H_ */
