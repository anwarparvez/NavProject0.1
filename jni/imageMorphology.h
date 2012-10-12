#include <cv.h>
#include<iostream>
using namespace cv;

class morphological{
  
public :
	IplImage* opening(IplImage *image,int rows,int cols,int shape,int iter);
	IplImage* closing(IplImage *image,int rows,int cols,int shape,int iter);
};
