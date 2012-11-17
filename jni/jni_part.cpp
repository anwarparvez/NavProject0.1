#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include "Log.hpp"
#include <iostream>


using namespace std;
using namespace cv;

extern "C" {
extern  IplImage* pImage;
JNIEXPORT void JNICALL Java_com_example_opencvtest_OpenCV_FindFeatures(JNIEnv*, jobject, jlong addrGray, jlong addrRgba)
{
	char logMsg[100];
    Mat* pMatGr=(Mat*)addrGray;
    Mat* pMatRgb=(Mat*)addrRgba;
    sbrc::Log::info("Log print");
    pImage=&((IplImage)(*pMatRgb));
    IplImage *img=pImage;
    sprintf(logMsg,"width=%d height=%d ",img->width,img->height);
    sbrc::Log::info(logMsg);
}

}
