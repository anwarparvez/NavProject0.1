
#include <iostream>
#include "imageMorphology.h"
#include "constants.h"
#include "com_example_opencvtest_OpenCV.h"
#include "segment.h"
#include<vector>
#include "voxel.h"
#include "RansacHeader.h"
#include "filters.h"
//#include "goodFeatureTotrack.cpp"
#include <android/log.h>
#include "imageROIextractor.h"
using namespace std;
#define ANDROID_LOG_VERBOSE ANDROID_LOG_DEBUG
#define LOG_TAG "CVJNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
IplImage* laplace(IplImage *);
Mat laplaceMat(cv::Mat &channel);
vector<CvPoint*> verticalLineholder;
vector<CvPoint*> otherLines;
Voxel *voxelGrid;
extern IplImage* pImage;
static bool isVertical = false;
int* imageMap;
inline int fRound(float flt){
  return (int) floor(flt+0.5f);
}
 bool sortOut(const CvPoint p1, const CvPoint p2){
	 return p1.x == p2.x ? p1.y < p2.y : p1.x < p2.x ; 
 }
 bool sortLines(const CvPoint* p1, const CvPoint* p2){
	  
	 //p1[0].x == p2[0].x ? (p1[0].y == p2[0].y ? (p1[1].x == p2[1].x ? p1[1].y > p1[1].y : p1[1].x < p2[1].x) : p1[0].y > p1[1].y) : p1[0].x < p2[0].x;
	 if(p1[0].x < p2[0].x && p1[1].x < p2[1].x)
		 return true;
	 if(p1[0].x > p2[0].x && p1[1].x > p2[1].x)
		 return false;
	 return false;
 }
 
 CvMat * circleOperation(IplImage *inputImage){

	IplImage* hsv = cvCreateImage(cvGetSize(inputImage),IPL_DEPTH_8U,3);
	cvCvtColor(inputImage,hsv,CV_RGB2HSV);
	cvNamedWindow("tempShow");

	CvMat* mask = cvCreateMat(hsv->height,hsv->width,CV_8UC1);
	cvInRangeS(hsv,cvScalar(0.14*256,0.60*256,0.20*256,0),cvScalar(1.0*256,1.0*256,1.0*256,0),mask);

	//cvReleaseImage(&hsv);
		cvShowImage("tempShow",mask);
		cvWaitKey(0);
	IplConvKernel* strucElem1 = cvCreateStructuringElementEx(21,21,10,10,CV_SHAPE_RECT,NULL);
	IplConvKernel* strucElem2 = cvCreateStructuringElementEx(11,11,5,5,CV_SHAPE_RECT,NULL);
	
	/*cvDilate(mask,mask,strucElem2);
	cvErode(mask,mask,strucElem1);*/
	cvErode(mask,mask,strucElem2);
	cvDilate(mask,mask,strucElem2);
	cvReleaseStructuringElement(&strucElem1);
	cvReleaseStructuringElement(&strucElem2);

	
	cvShowImage("tempShow",mask);
	cvWaitKey(0);

	return mask;
}
 void imageMapinit(IplImage *img,int size){
	 
	  /*imageMap = new int*[img->height];
	  for(int in = 0;in<img->height;in++)
		  imageMap[in] = new int[img->width];
	  for(int i =0 ;i<img->height;i++)
		  for(int j =0; j<img->width;j++)
			  imageMap[i][j] = 0;
	  printf("height = %d width = %d\n",img->height, img->width);*/
	 for(int i =0;i<size;i++)
		 imageMap[i] = 0;
 }
 ///////////////////////////////////////////////
 void displayLines(IplImage* imgSource){

	return;
}
 //////////////////////////////////////////////////////
 void imageResize(IplImage *srcImage, IplImage *destImage){

     cvPyrDown(srcImage,destImage);
 	return;
 }
 ////////////////////////////////////////////////////////
 IplImage* getReallocation(IplImage *src, int depth,int nchannel){
 	return cvCreateImage(cvGetSize(src), depth, nchannel);
 }
 //////////////////////////////////////////////////////////////
 int findVerticallines(vector<int> *indexHolder, CvSeq * linez){

	int ux[2] = {1,0};
    int u[2];
	int sum = 0;
	for(int i =0 ;i<indexHolder->size(); i++){
	   CvPoint* points = (CvPoint*)cvGetSeqElem(linez,indexHolder->at(i));
	   u[0] = abs(points[0].x - points[1].x);
	   u[1] = abs(points[0].y - points[1].y);
       sum += u[0]*ux[0] + u[1]*ux[1]; 
	}
	printf("The sum is :%d",sum);
	return sum < indexHolder->size()*10;
}

 JNIEXPORT void JNICALL Java_com_example_opencvtest_OpenCV_findEdgesandCorners(JNIEnv *env, jobject jmb)
 {


	//	cvNamedWindow("edgeImages");
		//cvNamedWindow("Normalized");
		morphological morph;
		//cvNamedWindow("cornerPoints");
		bool set = false;
	    IplImage* img = pImage;
		IplImage* imgOnwork = NULL;
	    IplImage* out = NULL;
	    IplImage* finalOut = NULL;
		vector<int> *indexHolder;

		// assert(img->width % 2 == 0 && img->height % 2 == 0);
		if(img->height >500 || img->width > 650){

	         imgOnwork = cvCreateImage(cvSize(img->width/2,img->height/2),img->depth,img->nChannels);
	         imageResize(img,imgOnwork);
			// imageMapinit(imgOnwork);
	         set = true;

		}else{
			imgOnwork = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
			//imageMapinit(imgOnwork);
			cvCopyImage(img,imgOnwork);
		}

	    //cvReleaseImage(&img);
		out = getReallocation(imgOnwork, imgOnwork->depth,imgOnwork->nChannels);
		finalOut = cvCreateImage(cvGetSize(imgOnwork),imgOnwork->depth, 1);
		IplImage* finalOutSecond = cvCreateImage(cvGetSize(imgOnwork),IPL_DEPTH_8U,1);
		IplImage* histImage = cvCreateImage(cvSize(300,240),8,1);
		cvSet(histImage,cvScalarAll(255),0);
		cout<< "image depth"<<imgOnwork->depth<<"\n";
		//cornerOut = getReallocation(imgOnwork,img->depth,1);
	    if(imgOnwork == NULL || out == NULL || finalOut == NULL){
	    	if(!finalOut)
	    	printf("We are exiting\n");
	    	exit(0);
	    }
		//cvSmooth(imgOnwork,out, CV_GAUSSIAN,3,3);// gaussian smoothing may not be the good one to introduce
		cvCopyImage(imgOnwork,out);
		//getTheSharpenedImage(out);
		//cvAdd(imgOnwork,out,out,NULL);
		//  showImage("edgeImages",out);
		 //.. waitKey(0);
	    cvCvtColor(out,finalOut,CV_RGB2GRAY);
		//cvSobel(finalOut,finalOutSecond,1,0,3);
		//cvAbs(finalOutSecond,finalOutSecond);
		//cvShowImage("edgeImages",finalOutSecond);
		//waitKey(0);
		cvCreateHistogram(finalOut,histImage);
	//	cvShowImage("edgeImages",finalOut);
		//waitKey(0);
		cvNormalize(finalOut,finalOutSecond,0.0,255.0,CV_MINMAX);
		cvCreateHistogram(finalOutSecond,histImage);
		//cvShowImage("Normalized",finalOutSecond);
		//waitKey(0);
		cvReleaseImage(&histImage);
		cvCopy(finalOutSecond,finalOut);
		//finalOutSecond = laplace(finalOutSecond);
		//cvAdd(finalOut,finalOutSecond,finalOut,NULL);
		cvSmooth(finalOut,finalOut,CV_GAUSSIAN,3,3);
		Mat gray1,gray2,gray3;
		Mat tmpFinalOut = (Mat)finalOut;
		getEdges(tmpFinalOut,gray1,gray2);
		cv::add(gray1,gray2,gray3);
		//cv::Canny(gray3,gray3,30,100,3);
		gray3.convertTo(gray1,CV_8U,255);
		//cv::Canny(gray1,gray1,30,100);
		finalOut = &((IplImage)gray1);
		finalOut = morph.closing(finalOut,3,3,0,1);
		//..cvShowImage("edgeImages",finalOut);
		//waitKey(0);
		//cvCanny(finalOut,finalOut,30,100,3);//10,10
		//finalOut = morph.closing(finalOut,3,3,0,1);
		//cvShowImage("edgeImages",finalOut);
		//waitKey(0);

		//HoughLineSegment(finalOut);
		IplImage* gray = cvCreateImage(cvGetSize(imgOnwork), IPL_DEPTH_8U, 1);
		cvCvtColor(imgOnwork,gray,CV_RGB2GRAY);
		CvMemStorage* store = cvCreateMemStorage(0);
				//CvMat* mat = cvCreateMat(1,imgOnwork->height*imgOnwork->width,CV_32FC4);
		char c;
		c ='l';// getchar();
		CvSeq* linez = NULL;
		CvSeq* circles = NULL;
		if(c == 'l' || c == 'L')
			linez = cvHoughLines2(finalOut,store,CV_HOUGH_PROBABILISTIC,1.0,CV_PI/180,100,40.0,20.0);//20,10.0,10.0
		else{
			CvMat* cvmat = circleOperation(imgOnwork);
			cvCopy(cvmat,finalOut,NULL);
			cvSmooth(finalOut, finalOut, CV_GAUSSIAN, 15, 15, 0, 0);
			circles = cvHoughCircles(finalOut,store,CV_HOUGH_GRADIENT,1.3,finalOut->height/10,100,40,0,0);
		}
		if(linez == NULL || linez->total < 0){
		 printf("Programe is exiting\n");
		 printf("Evacuate now\n");
	     exit(0);
		}

	    int size =  linez == NULL ? circles->total : linez->total;
		printf("linez size %d\n",linez->total);
		//cvReleaseImage(&finalOut);
		
		imageMap = new int[linez->total];
		imageMapinit(imgOnwork,linez->total);
		int trailRunner = 0;
		int colorVal = 255;
		isVertical = false;
		displayLines(finalOut);
		while(trailRunner++ < 3){
		  //IplImage*  imgForoutput = cvCreateImage(cvGetSize(imgOnwork),imgOnwork->depth,imgOnwork->nChannels);
		  //cvCopyImage(img,imgForoutput);
		  printf("Start Ransac\n");
	      indexHolder = initiateVanishing(linez, imageMap);

		if(findVerticallines(indexHolder, linez)){
	           printf("Found the vertical lines\n");
			   for(int i = 0; i < indexHolder->size(); i++){
					  CvPoint* pointtoHold = new CvPoint[2];
					  CvPoint* tempHold = (CvPoint*) cvGetSeqElem(linez,indexHolder->at(i));
					  pointtoHold[0] = tempHold[0];
					  pointtoHold[1] = tempHold[1];
					  verticalLineholder.push_back( pointtoHold);
					  cvLine(imgOnwork,verticalLineholder.at(verticalLineholder.size()-1)[0],verticalLineholder.at(verticalLineholder.size()-1)[1],cvScalar(255,0,colorVal),1,CV_AA,0);
			   }

				  isVertical = true;
		 }
		 else{
		  for(int i = 0;i<indexHolder->size();i++){

			/*float rho = points[0];
			  float theta = points[1];
			  double a = cos(theta);
			  double b= sin(theta);
			  double x0 = rho*a;double y0 = b*rho;
			  cv::Point ptr1(cvRound(x0 + 50*(-b)),cvRound(y0 + 50*(a)));
			  cv::Point ptr2(cvRound(x0-50*(-b)),cvRound(y0 - 50*(a)));*/
			  CvPoint* tempVarPoint = new CvPoint[2];
			  if(c == 'l' || c == 'L'){
				 CvPoint* points= (CvPoint*) cvGetSeqElem(linez,indexHolder->at(i));
				 tempVarPoint[0] = points[0];
				 tempVarPoint[1] = points[1];
				 otherLines.push_back(tempVarPoint);

				 if(trailRunner == 1)
					 cvLine(imgOnwork,points[0],points[1],cvScalar(0,255,0),1,CV_AA,0);
				 else if (trailRunner == 2)
					 cvLine(imgOnwork,points[0],points[1],cvScalar(255,0,colorVal),1,CV_AA,0);
				 else
					 cvLine(imgOnwork,points[0],points[1],cvScalar(0,127,colorVal),1,CV_AA,0);
			  }
			  else{
					float* floatpoints = (float*) cvGetSeqElem(circles,i);
					CvPoint cvPoints = cvPoint(cvRound(floatpoints[0]),cvRound(floatpoints[1]));
					CvScalar val = cvGet2D(finalOut, cvPoints.y,cvPoints.x);
					if(val.val[0]<1) continue;
					   cvCircle(imgOnwork,cvPoints,cvRound(floatpoints[2]),CV_RGB(255,0,0),1,CV_AA,0);
			  }

				
		  }
		 }
		  for(int i = 0;i < indexHolder->size(); i++ )
			  imageMap[indexHolder->at(i)] = 1;
		 // showImage("edgeImages",imgOnwork);
		  //cvWaitKey(0);
		}

		//cvCvtColor(imgOnwork,cornerOut,CV_RGB2GRAY);
	     // showImage("edgeImages",imgOnwork);
		  //cvWaitKey(0);
		 // cvReleaseImage(&imgOnwork);

		//showImage("cornerPoints",finalOut);
	    cout<< "size of other lines " <<otherLines.size()<<"\n";
		cout<< "size of vertical lines "<<verticalLineholder.size()<<"\n";

		//cvCopyImage(imgOnwork,img);
		cvReleaseMemStorage(&store);
		cvReleaseImage(&out);

	 return;

}

 int getCornerpoints(){

 	IplImage* img = pImage;
 	IplImage* cornerOut = NULL;
 	IplImage* imgOnwork = NULL;
 	IplImage* R = NULL;
 	IplImage* G = NULL;
 	IplImage* B = NULL;
 	IplImage* R2 = NULL;
 	IplImage* G2 = NULL;
 	IplImage* B2 = NULL;
 	IplImage* sourceFrame = NULL;
 	CvScalar harris;
 	//cvNamedWindow("SharpedImage");
 	//cvNamedWindow("SourceImage");
 	assert(img->width % 2 == 0 && img->height % 2 == 0);
 		if(img->height >500 || img->width > 650){

 	         imgOnwork = cvCreateImage(cvSize(img->width/2,img->height/2),img->depth,img->nChannels);
 	         imageResize(img,imgOnwork);

 		}
 		else{
 			imgOnwork = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
 			cvCopyImage(img,imgOnwork);
 		}

         IplImage* result = cvCreateImage(cvGetSize(imgOnwork),imgOnwork->depth, imgOnwork->nChannels);
         IplImage* result2 = cvCreateImage(cvGetSize(imgOnwork),imgOnwork->depth, imgOnwork->nChannels);
         R = cvCreateImage(cvGetSize(imgOnwork), imgOnwork->depth,1);
         G = cvCreateImage(cvGetSize(imgOnwork), imgOnwork->depth,1);
         B = cvCreateImage(cvGetSize(imgOnwork), imgOnwork->depth,1);
         R2 = cvCreateImage(cvGetSize(imgOnwork), imgOnwork->depth,1);
         G2 = cvCreateImage(cvGetSize(imgOnwork), imgOnwork->depth,1);
         B2 = cvCreateImage(cvGetSize(imgOnwork), imgOnwork->depth,1);
 		printf("image depth: %d\n", imgOnwork->depth);
 		cornerOut = cvCreateImage(cvGetSize(imgOnwork),imgOnwork->depth, 1);
         sourceFrame = cvCreateImage(cvGetSize(imgOnwork),imgOnwork->depth, imgOnwork->nChannels);
         cvCopyImage(imgOnwork,sourceFrame);
 		cvSplit(imgOnwork,B,G,R, NULL);
 		B2 = laplace(B);
 		G2 = laplace(G);
 		R2 = laplace(R);
 		cvMerge(R2,G2,B2,NULL,result);
 		cvAdd(sourceFrame,result,result2,NULL);
 		cvCvtColor(result2,cornerOut,CV_RGB2GRAY);
 		Mat gray1,gray2,gray3;
 		vector<Mat> matCollector;
 		Mat tmpcornerOut = (Mat)cornerOut;
 		getEdges(tmpcornerOut,gray1,gray2);
 		printf("Hello got it\n");
 		/*matCollector.push_back(gray1);
 		matCollector.push_back(gray2);
 		cv::merge(matCollector,gray3);*/
 		cv::add(gray1,gray2,gray3);
 		//matCollector.clear();
 		gray3.convertTo(gray1,CV_32FC1);
 		//gray1.reshape(1,cornerOut ->height);
 		printf("%d\n", gray1.depth() );
 		cornerOut =&((IplImage)gray1);
 		//imshow("getthehell",gray3);
 		//waitKey(0);
         printf("this is point is okay\n");
 	CvMat* mat = cvCreateMat(sourceFrame->height,sourceFrame->width, CV_32FC1);
 	cvSmooth(cornerOut,cornerOut,CV_GAUSSIAN,11,11);
 	cvCornerHarris(cornerOut,mat,3);
 	printf("number of vertical lines %d\n",verticalLineholder.size());
 	vector<CvPoint> pointsCol;
 	if(verticalLineholder.size() != 0)
 	   pointsCol = getFilteredpoints(mat,verticalLineholder);
 	else
 		pointsCol = getFilteredpoints(mat,otherLines);
 	printf("right before the voxel creation %d\n",pointsCol.size());
 	voxelGrid = new Voxel( &pointsCol, 25, 25, 0, pointsCol.size(), 2, 10);
 	voxelGrid->maxminFinder();
 	voxelGrid->gridCreation();
 	voxelGrid->histogramGeneration();
 	voxelGrid->voxelFiltering();
 	printf("size of the filtered corner: %d\n",pointsCol.size());
 	printf("size of the listOfPoints %d\n",voxelGrid->listofPoints.size());
 	sort(voxelGrid->listofPoints.begin(),voxelGrid->listofPoints.end(),sortOut);
 	for(int i = 0; i < voxelGrid->listofPoints.size()/*pointsCol.size()*/; i++){
 			//for(int j =0 ;j<cornerOut->width;j++){
 	        //   harris =  cvGet2D(mat,i,j);
 	             // if(harris.val[0] > 10e-06)
 			//cvCircle(result,cvPoint(pointsCol.at(i).y,pointsCol.at(i).x),fRound(1),cvScalar(25,0,255),1);
 		printf("point x = %d y = %d\n",voxelGrid->listofPoints.at(i).x,voxelGrid->listofPoints.at(i).y);
 			cvCircle(result,cvPoint(voxelGrid->listofPoints.at(i).x,voxelGrid->listofPoints.at(i).y),fRound(3),cvScalar(25,0,255),1);
 	}
 			//}
 	cout<<"seize of the other lines "<< otherLines.size()<<"\n";
 	verticalLineholder.insert(verticalLineholder.end(),otherLines.begin(),otherLines.end());
 	//sort(verticalLineholder.begin(),verticalLineholder.end(),sortLines);
 	printf("Total lines in the system are : %d\n",verticalLineholder.size());
 		//cvShowImage("SharpedImage",result);
 		//cvShowImage("SourceImage",sourceFrame);
 	//	cvWaitKey(0);
         cvReleaseImage(&result);
         cvReleaseImage(&result2);
 		// cvReleaseImage(&sourceFrame);
         cvReleaseImage(&R);
         cvReleaseImage(&G);
         cvReleaseImage(&B);
      //    cvReleaseImage(&R2);
       //   cvReleaseImage(&G2);
          //cvReleaseImage(&B2);
          return 0;

 }

JNIEXPORT jint JNICALL Java_com_example_opencvtest_OpenCV_getCornerpoints
  (JNIEnv *env, jobject obj){
	getCornerpoints();
    imageROIExtractor(pImage,verticalLineholder,voxelGrid->listofPoints);
	return 0;
}
 IplImage* laplace(IplImage* channel) {

     CvSize size = cvSize(channel->width, channel->height);

     IplImage* temp = cvCreateImage(size, channel->depth, 1);

     IplImage* lapl = cvCreateImage(size, channel->depth, 1);

     int width = size.width;

     int height = size.height;

     cvConvertScale(channel, temp, 1.0);

     CvMat* ker = cvCreateMat(3, 3, CV_32FC1);

     cvSet(ker, cvScalarAll(-1.0));

     cvSet2D(ker, 1, 1, cvScalarAll(8.0));

     printf("this is been executed\n");

     cvFilter2D(temp, lapl, ker);

     cvReleaseMat(&ker);

     double maxv = -1.79769e-308;

     float maxFloat = 1.79769e+308;

     double minv = maxFloat;

     cvMinMaxLoc(lapl, &minv, &maxv);


     for (int i = 0; i < width * height; i++) {

         double lap_val = cvGet1D(lapl, i).val[0];

         int v = (int) ((255.0 * lap_val / maxv) + 0.5); // this calculation does nothing particularly

         cvSet1D(channel, i, cvScalarAll(v));
     }

     maxv = 0.0;

     cvMinMaxLoc(channel, &minv, &maxv);

     for (int i = 0; i < width * height; i++) {

         double val = cvGet1D(channel, i).val[0];
         int v = (int) ((255.0 * val / maxv) + 0.5);
         cvSet1D(channel, i, cvScalarAll(v));
     }

    cvReleaseImage(&temp);
     cvReleaseImage(&lapl);
     cvReleaseMat(&ker);

     return channel;
 } // end of function

 ////////////////////
 Mat laplaceMat(Mat &channel){

 	cv::Size size(channel.cols,channel.rows);
 	Mat temp(size,CV_32FC1);
 	Mat lapl(size,CV_32FC1);
 	int width = size.width;
 	int height = size.height;
 	channel.convertTo(temp,CV_32FC1,1.0/255.0);
 	Mat ker(3,3,CV_32FC1);
 	ker.setTo(cv::Scalar(-1.0));
 	ker.at<float>(1,1) = 8.0;
 	imshow("tempS",temp);
 	cv::filter2D(temp,lapl,-1,ker,Point(-1,-1),0,BORDER_CONSTANT);
 	double maxv = -1.79769e-308;
 	float maxFloat = 1.79769e+308;
 	double minv = maxFloat;
 	cv::minMaxLoc(lapl, &minv, &maxv);
 	//Mat channelTemp(size,CV_32FC1);
 	if(channel.rows == 0){
 	   printf("Programme exiting");
 	   exit(0);
 	}
 	lapl.convertTo(channel,CV_8UC1,255.0);
 //	imshow("convertedShow",channel);
 	//waitKey(0);
     /*printf("the max value %f\n",maxv);
 	for (int i = 0; i < height; i++) {
 		for(int j =0; j<width; j++){

 			double lap_val = lapl.at<float>(i,j);//cvGet1D(lapl, i).val[0];
 			int v = (int) ((255.0 * lap_val / maxv) + 0.5); // this calculation does nothing particularly
 			channel.at<uchar>(i,j) = v;
 		}
         //cvSet1D(channel, i, cvScalarAll(v));
     }
 	imshow("tempShow1",channel);
     maxv = 0.0;

 	cv::minMaxLoc(channel, &minv, &maxv);
     for (int i = 0; i < height; i++) {
 		for(int j =0; j < width; j++){
 			double val = channel.at<uchar>(i,j);
 			int v = (int) ((255.0 * val / maxv) + 0.5);
 			channel.at<uchar>(i,j) = v;
 		}
     }

 	imshow("tempShow2",channel);
     waitKey(0);
 	lapl.copyTo(channel);*/
 	return  channel;
 }

#ifdef __cplusplus
}
#endif
