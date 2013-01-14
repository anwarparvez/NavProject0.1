LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_LIB_TYPE :=STATIC
include D:/others/final-blind/OpenCV-2.4.2-android-sdk/OpenCV-2.4.2-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := main_object
LOCAL_LDLIBS +=  -llog -ldl
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_SRC_FILES := Log.cpp jni_part.cpp  cvjni.cpp Morphological.cpp EdgeFinder.cpp RansacProcessor.cpp CornerFinder.cpp Voxel.cpp ROIExtractor.cpp ConvexHull.cpp WaterShad.cpp

#LOCAL_STATIC_LIBRARIES := cxcore cv

include $(BUILD_SHARED_LIBRARY)
