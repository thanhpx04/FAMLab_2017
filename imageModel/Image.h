/*
 * Image.h
 *
 *  Created on: Sep 16, 2016
 *      Author: linh
 */

#ifndef IMAGE_H_
#define IMAGE_H_



class Image {
private:
	std::string fileName;
	std::vector<Edge> listOfEdges;
	std::vector<Point> listOfMLandmarks;
public:
	Image();
	virtual ~Image();
	Image(std::string);
	void setFileName(std::string);
	std::string getFileName();
	void setEdges(std::vector<Edge>);
	std::vector<Edge> getEdge();
};

#endif /* IMAGE_H_ */
