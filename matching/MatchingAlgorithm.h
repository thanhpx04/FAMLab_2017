#ifndef MATCHINGALGORITHM_H
#define MATCHINGALGORITHM_H

#include <cmath>
#include <vector>
using namespace std;

#include "MAELab/imageModel/Point.h"
#include "MAELab/imageModel/Edge.h"

vector<vector<int> > cumulativeDistanceMatrix(vector<int> vectorX, vector<int> vectorY);
vector< pair<int,int> > optimalWarpingPath(vector< vector<int> > dtw);

vector<Point> findMinXmappingY(Edge edge);
vector<Point> findMaxXmappingY(Edge edge);
vector<Point> getSubPointsByDistance(vector<Point> listPoints, int distance);
vector<int> getXvalueOfListPoints(vector<Point> listPoints);

#endif // MATCHINGALGORITHM_H
