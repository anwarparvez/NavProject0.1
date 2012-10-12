#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<vector>
#include<set>
#include<map>
#include<cstdlib>
#include<cv.h>
#include<algorithm>
#include<string.h>
using namespace std;
using namespace cv;

int convexHull(vector<CvPoint> X,bool onEdge);
int getCrossProduct(CvPoint,CvPoint,CvPoint);
int getDotProduct(CvPoint,CvPoint);
int perpLex(vector<CvPoint> X);
