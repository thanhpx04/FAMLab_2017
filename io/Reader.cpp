#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

#include "../imageModel/Point.h"

std::vector<ptr_Point> readTPS(const char* filename)
{
	ifstream openFile(filename);
	vector<ptr_Point> mLandmarks;
	string lineText;
	if(openFile.is_open())
	{
		getline(openFile,lineText);
		cout<<(char*)lineText.c_str();
		char* fline = strtok((char*)lineText.c_str(),"=");

		int nPoints = (fline[3] - '0') * 10 + (fline[4] -'0' );
		cout<<"n points: "<<nPoints;
		int i =0;
		while(i < nPoints)
		{
			getline(openFile,lineText);
			fline = strtok((char*)lineText.c_str()," ");
			mLandmarks.push_back(new Point((int)fline[0],(int)fline[1]));
			i++;
		}
	}else
	{
		cout<<endl<<"Cannot open this file !";
	}
	openFile.close();
	return mLandmarks;
}

