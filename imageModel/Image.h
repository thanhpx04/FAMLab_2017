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
	//std::vector<Edge> listOfEdges;
	std::vector<ptr_Point> listOfMLandmarks;
	ptr_IntMatrix grayMatrix;
	ptr_RGBMatrix imgMatrix;
	ptr_IntMatrix grayHistogram;
	float medianHistogram = 0;
	float meanHistogram = 0;
	float thresholdValue = 0;

	void calcGrayHistogram();
	void calThresholdValue();

public:
	Image();
	virtual ~Image();
	Image(std::string);
	void setFileName(std::string);
	std::string getFileName();
	void setMLandmarks(string);
	ptr_IntMatrix getGrayMatrix();
	ptr_RGBMatrix getRGBMatrix();
	float getMedianHistogram();
	float getMeanHistogram();
	float getThresholdValue();
	//void setEdges(std::vector<Edge>);
	//std::vector<Edge> getEdge();
};

#endif /* IMAGE_H_ */
