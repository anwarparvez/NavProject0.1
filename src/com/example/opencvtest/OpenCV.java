package com.example.opencvtest;

public class OpenCV {
	static{
		System.loadLibrary("main_object");
	}
	public native boolean setSourceImage(int[] pixels, int width, int height,String str);
	public native byte[] getSourceImage();
	public native void extractSURFFeature();
	public native int findEdgesandCorners();
	public native int getCornerpoints();
	
	
}
