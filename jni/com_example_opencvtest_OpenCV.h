/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_opencvtest_OpenCV */

#ifndef _Included_com_example_opencvtest_OpenCV
#define _Included_com_example_opencvtest_OpenCV
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_opencvtest_OpenCV
 * Method:    setSourceImage
 * Signature: ([III)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_opencvtest_OpenCV_setSourceImage
  (JNIEnv *, jobject, jintArray, jint, jint);

/*
 * Class:     com_example_opencvtest_OpenCV
 * Method:    getSourceImage
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_example_opencvtest_OpenCV_getSourceImage
  (JNIEnv *, jobject);

/*
 * Class:     com_example_opencvtest_OpenCV
 * Method:    extractSURFFeature
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_opencvtest_OpenCV_extractSURFFeature
  (JNIEnv *, jobject);

/*
 * Class:     com_example_opencvtest_OpenCV
 * Method:    findEdgesandCorners
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_opencvtest_OpenCV_findEdgesandCorners
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif