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
#include<exception>
using namespace std;
using namespace cv;

#define INF 500000
int convexHull(vector<CvPoint> X,bool onEdge);
int getCrossProduct(CvPoint,CvPoint,CvPoint);
int getDotProduct(CvPoint,CvPoint);
int perpLex(vector<CvPoint> X);
bool sortOutPoints(const CvPoint, const CvPoint);
double getNormCrossProduct(CvPoint Xi, CvPoint next, CvPoint P);
int perpLexU(vector<CvPoint> X);
