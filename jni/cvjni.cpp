/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include<vector>
#include <jni.h>
#include <android/log.h>

#include "bmpfmt.h"
#include "com_samsung_indoornavigation_opencv_OpenCV.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "Log.hpp"
#include<stdio.h>
#include<iostream>
#include<cmath>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "EdgeFinder.h"
#include "CornerFinder.h"
#include "ROIExtractor.h"

using namespace std;

#define MAX 1000000
#define FOR(x,a,b) for(int x = a; x<=b; x++)

using namespace std;
using namespace cv;

#define ANDROID_LOG_VERBOSE ANDROID_LOG_DEBUG
#define LOG_TAG "CVJNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
IplImage* pImage=NULL;

IplImage* loadPixels(int* pixels, int width, int height);
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,
		jint width, jint height);

int comp(const void *a ,const void *b){
	if(*((double*)a)>*((double *)b))
		return 1;
	else
		return -1;

}
void randomGenerator(void){

  srand(time(NULL));
  int val;
  double memo[2002];
  memset(memo,-1,sizeof(memo));
  int counter = 0;
  while((val = rand())){
	  memo[counter++] = (double)val*0.0000002;
      if(counter == 2000)
		break;
  }
  qsort(memo,counter,sizeof(memo[0]),comp);
  int i = 0;
  FOR(i,0,counter-1)
  sbrc::Log::info("%lf\n",memo[i]);
  return;
}

JNIEXPORT jboolean JNICALL Java_com_samsung_indoornavigation_opencv_OpenCV_setSourceImage3(
		JNIEnv * env, jobject thiz, jstring javaString,jlong addrRgba){

		sbrc::Log::info("set Image Start");
	    Mat* pMatRgb=(Mat*)addrRgba;
		//Do something with the Image
	   // rectangle(*pMatRgb, cvPoint(20, 15), cvPoint(100, 70), cvScalar(255, 0, 0, 0),1, 8, 0);

	    Mat src=pMatRgb->clone();
	    IplImage* img = &(IplImage)src;

	    //imshow("EdgeFinder",(Mat) ef.doProcess());
		if (img == NULL) {
			sbrc::Log::info("Image load Failed");
			return 0;
		}

	    sbrc::Log::info("clone done");
	    EdgeFinder ef(img);
	    sbrc::Log::info("EdgeFinder  init done");
	    ef.doProcess();

		sbrc::Log::info("Load Image Done.");

		CornerFinder cf;
		cf.setVeticalLineHolder(ef.getVeticalLineHolder());
		sbrc::Log::info("setVeticalLineHolder Done.");
		cf.setOtherLineHolder(ef.getOtherLineHolder());
		sbrc::Log::info("cf.setOtherLineHolder Done.");
		cf.getCornerpoints(img);

		sbrc::Log::info(".getCornerpoints Done.");

	/*	vector<CvPoint> superContainer=cf.getVoxelGrid()->listofPoints;
		for(int i = 0; i < superContainer.size(); i++){
		circle(*pMatRgb,cvPoint(superContainer.at(i).x,superContainer.at(i).y),1,cvScalar(25,i*200,255),2);
		}*/

		ROIExtractor roiExtractor;

		/*vector<vector<pair<CvPoint, int> > > imageROIExtractor(
				vector<CvPoint*>&lineContainer, vector<CvPoint> cornerPoints,
				IplImage *img, Mat *imgMat);*/

		vector<CvPoint*> lineContainer=cf.getVeticalLineHolder();
		vector<CvPoint> cornerPoints=cf.getVoxelGrid()->listofPoints;

		vector < vector < pair<CvPoint, int > > > container = roiExtractor.imageROIExtractor(lineContainer,cornerPoints,( IplImage* ) img,(Mat *)pMatRgb);

		return 1;
}

JNIEXPORT jboolean JNICALL Java_com_samsung_indoornavigation_opencv_OpenCV_setSourceImage(
		JNIEnv * env, jobject thiz, jintArray photo_data, jint width,
		jint height, jstring javaString) {

    //Get the native string from javaString
    const char *nativeString = env->GetStringUTFChars(javaString, 0);

    //Do something with the nativeString


    sbrc::Log::info("set Image Start");
	//randomGenerator();
	   // use your string
    sbrc::Log::info("Image path %s",nativeString);

	if (pImage != NULL) {
		cvReleaseImage(&pImage);
		   sbrc::Log::info("ReleaseImage ");
		pImage = NULL;
	}
	//pImage = getIplImageFromIntArray(env, photo_data, width, height);
	 pImage = cvLoadImage(nativeString);
	if (pImage == NULL) {
		   sbrc::Log::info("Image load Failed");
		return 0;
	}
	//cvRectangle(pImage, cvPoint(20, 15), cvPoint(100, 70), cvScalar(255, 0, 0, 0),1, 8, 0);

	 sbrc::Log::info("Load Image Done.");
    //DON'T FORGET THIS LINE!!!
    env->ReleaseStringUTFChars(javaString, nativeString);
	return 1;
}
JNIEXPORT jbyteArray JNICALL Java_com_samsung_indoornavigation_opencv_OpenCV_getSourceImage(
		JNIEnv* env, jobject thiz) {
	if (pImage == NULL) {
		LOGE("No source image.");
		return 0;
	}
	cvFlip(pImage);
	int width = pImage->width;
	int height = pImage->height;
	int rowStep = pImage->widthStep;
	int headerSize = 54;
	int imageSize = rowStep * height;
	int fileSize = headerSize + imageSize;
	unsigned char* image = new unsigned char[fileSize];
	struct bmpfile_header* fileHeader = (struct bmpfile_header*) (image);
	fileHeader->magic[0] = 'B';
	fileHeader->magic[1] = 'M';
	fileHeader->filesz = fileSize;
	if(fileHeader->filesz == 0)
	{
		LOGE("Error in image file size\n");
		return 0;

	}
	fileHeader->creator1 = 0;
	fileHeader->creator2 = 0;
	fileHeader->bmp_offset = 54;
	struct bmp_dib_v3_header_t* imageHeader =
			(struct bmp_dib_v3_header_t*) (image + 14);
	imageHeader->header_sz = 40;
	imageHeader->width = width;
	imageHeader->height = height;
	imageHeader->nplanes = 1;
	imageHeader->bitspp = 24;
	imageHeader->compress_type = 0;
	imageHeader->bmp_bytesz = imageSize;
	imageHeader->hres = 0;
	imageHeader->vres = 0;
	imageHeader->ncolors = 0;
	imageHeader->nimpcolors = 0;
	memcpy(image + 54, pImage->imageData, imageSize);
	jbyteArray bytes = env->NewByteArray(fileSize);
	if (bytes == 0) {
		LOGE("Error in creating the image.");
		delete[] image;
		return 0;
	}
	env->SetByteArrayRegion(bytes, 0, fileSize, (jbyte*) image);
	delete[] image;
	return bytes;
}

IplImage* loadPixels(int* pixels, int width, int height) {
	int x, y;
	IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	unsigned char* base = (unsigned char*) (img->imageData);
	unsigned char* ptr;
	for (y = 0; y < height; y++) {
		ptr = base + y * img->widthStep;
		for (x = 0; x < width; x++) {
			// blue
			ptr[3 * x] = pixels[x + y * width] & 0xFF;
			// green
			ptr[3 * x + 1] = pixels[x + y * width] >> 8 & 0xFF;
			// blue
			ptr[3 * x + 2] = pixels[x + y * width] >> 16 & 0xFF;
		}
	}
	return img;
}
IplImage* getIplImageFromIntArray(JNIEnv* env, jintArray array_data,
		jint width, jint height) {
	int *pixels = env->GetIntArrayElements(array_data, 0);
	if (pixels == 0) {
		LOGE("Error getting int array of pixels.");
		return 0;
	}
	IplImage *image = loadPixels(pixels, width, height);
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	if (image == 0) {
		LOGE("Error loading pixel array.");
		return 0;
	}
	return image;
}

#ifdef __cplusplus
}
#endif

