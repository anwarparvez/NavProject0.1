#include "imageMorphology.h"

IplImage* morphological::closing(IplImage *image,int rows,int cols,int shape,int iter){
	int structure = 0;
	IplImage* dest = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
	if(shape == 0)
		structure = CV_SHAPE_RECT;
	else if(shape == 1)
		structure = CV_SHAPE_CROSS;
	else
		structure = CV_SHAPE_ELLIPSE;
	IplConvKernel *kernel = cvCreateStructuringElementEx(rows,cols,rows/2,cols/2,CV_SHAPE_RECT,NULL); 
	cvDilate(image,dest,kernel,iter);
	//cvShowImage("tempOpeningShow",dest);
	//cvWaitKey(0);
	cvErode(dest,image,kernel,iter);
	return image; 
}
IplImage* morphological::opening(IplImage *image,int rows,int cols,int shape,int iter){

	int structure = 0;
	//cvWaitKey(0);
	IplImage* dest = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
	if(shape == 0)
		structure = CV_SHAPE_RECT;
	else if(shape == 1)
		structure = CV_SHAPE_CROSS;
	else
		structure = CV_SHAPE_ELLIPSE;
	IplConvKernel *kernel = cvCreateStructuringElementEx(rows,cols,rows/2,cols/2,CV_SHAPE_RECT,NULL); 
	
	cvErode(image,dest,kernel,iter);
	//cvShowImage("tempOpeningShow",dest);
	//cvWaitKey(0);
	cvDilate(dest,image,kernel,iter);
	return image;
	
}
