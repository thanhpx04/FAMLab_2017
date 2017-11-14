#include "MatchingAlgorithm.h"


vector<vector<int> > cumulativeDistanceMatrix(vector<int> vectorX, vector<int> vectorY)
{
    int n = vectorX.size();
    int m = vectorY.size();
    // initial matrix DTW
    vector< vector<int> > dtw;
    dtw.resize(n);
    for (int i = 0; i < n; ++i)
        dtw[i].resize(m);
    //====================
    dtw[0][0] = 0;
    for(int i = 1; i < n; i++)
    {
        dtw[i][0] = dtw[i-1][0] + abs(vectorX[i] - vectorY[0]);
    }
    for(int j = 1; j < m; j++)
    {
        dtw[0][j] = dtw[0][j-1] + abs(vectorX[0] - vectorY[j]);
    }
    for(int i = 1; i < n; i++)
    {
        for(int j = 1; j < m; j++)
        {
            dtw[i][j] = abs(vectorX[i] - vectorY[j]) + min( min(dtw[i-1][j],dtw[i][j-1]),dtw[i-1][j-1]);
        }
    }
    return dtw;
}

vector< pair<int,int> > optimalWarpingPath(vector< vector<int> > dtw)
{
    vector< pair<int,int> > path;
    int i = dtw.size() - 1;
    int j = dtw[0].size() - 1;
    // the first point of backtracking
    path.push_back(make_pair(i,j));

    while (i>0 || j>0)
    {
        if(i==0)
            j=j-1;
        else if(j==0)
            i=i-1;
        else
        {
            if(dtw[i-1][j] == min(min(dtw[i-1][j],dtw[i][j-1]),dtw[i-1][j-1]))
                i=i-1;
            else if (dtw[i][j-1] == min(min(dtw[i-1][j],dtw[i][j-1]),dtw[i-1][j-1]))
                j=j-1;
            else
            {
                i=i-1;
                j=j-1;
            }
        }
        path.push_back(make_pair(i,j));
    }
    return path;
}
