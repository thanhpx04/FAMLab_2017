#ifndef MATCHINGALGORITHM_H
#define MATCHINGALGORITHM_H

#include <cmath>
#include <vector>
using namespace std;

vector< vector<int> > cumulativeDistanceMatrix(vector<int> vectorX, vector<int> vectorY);
vector< pair<int,int> > optimalWarpingPath(vector< vector<int> > dtw);

#endif // MATCHINGALGORITHM_H
