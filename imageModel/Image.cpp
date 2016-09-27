/*
 * Image.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: linh
 */
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>

using namespace std;

#include "Point.h"
#include "Line.h"
#include "Edge.h"
#include "Image.h"

Image::Image() {
	// TODO Auto-generated constructor stub

}

Image::~Image() {
	// TODO Auto-generated destructor stub
}
Image::Image(std::string filePath){
	fileName = filePath;
}
void Image::setFileName(std::string filePath){
	fileName = filePath;
}
std::string Image::getFileName(){
	return fileName;
}
void Image::setEdges(std::vector<Edge> edges){
	listOfEdges = edges;
}
std::vector<Edge> Image::getEdge(){
	return listOfEdges;
}
