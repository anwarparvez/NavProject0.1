package com.example.opencvtest;

public class OpenCV {
	static{
		System.loadLibrary("main_object");
	}
	public native boolean setSourceImage(int[] pixels, int width, int height,String path);
	public native byte[] getSourceImage();
	public native void extractSURFFeature();
	public native void findEdgesandCorners(long  matAddrRgba);
	public native int getCornerpoints();
    public native void FindFeatures(long matAddrGr, long matAddrRgba);
	
	
}
