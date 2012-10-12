#include<cv.h>
#include<vector>
#include<math.h>
#include <iostream>
using namespace std;
using namespace cv;
#ifdef __cplusplus
extern "C" {
#endif


vector<cv::Point2f > goodFeatureTracking(Mat image,Mat source){

	Mat tempMat;
	int maxCorners = 500;
	vector<cv::Point2f> corners;
	//corners.reserve(maxCorners);
	if(image.channels() != 1)
		cv::cvtColor(image,tempMat,CV_BGR2GRAY,1);
	else
		image.copyTo(tempMat);
	goodFeaturesToTrack(tempMat,corners,maxCorners,0.001,5,Mat(),3,false,0.04);
	//cv::cornerSubPix(tempMat,corners,cv::Size(11,11),cv::Size(3,3),cv::TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03 ));
	for(int i = 0; i < corners.size(); i++){
		circle(source,corners.at(i),2,cvScalar(0,0,255),2);
	}
	//cv::imshow("Tracker",source);
	//waitKey(0);
	return corners;
}

#ifdef __cplusplus
}
#endif
