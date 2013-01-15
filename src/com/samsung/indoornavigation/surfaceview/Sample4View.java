package com.samsung.indoornavigation.surfaceview;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import com.samsung.indoornavigation.opencv.OpenCV;

public class Sample4View extends SampleViewBase {

	public OpenCV mOpencv = new OpenCV();

	private Mat mYuv;
	private Mat mRgba;


	private Bitmap mBitmap;

	public Sample4View(Context context) {
		super(context);
	}

	@Override
	protected void onPreviewStarted(int previewWidtd, int previewHeight) {
		// initialize Mats before usage
		mYuv = new Mat(getFrameHeight() + getFrameHeight() / 2,
				getFrameWidth(), CvType.CV_8UC1);
		mRgba = new Mat();
		mBitmap = Bitmap.createBitmap(previewWidtd, previewHeight,
				Bitmap.Config.ARGB_8888);
	}

	@Override
	protected void onPreviewStopped() {

		if (mBitmap != null) {
			mBitmap.recycle();
			mBitmap = null;
		}

		// Explicitly deallocate Mats
		if (mYuv != null)
			mYuv.release();
		if (mRgba != null)
			mRgba.release();


		mYuv = null;
		mRgba = null;
	
	}

	@Override
	protected Bitmap processFrame(byte[] data) {
		mYuv.put(0, 0, data);
		Imgproc.cvtColor(mYuv, mRgba, Imgproc.COLOR_YUV420sp2RGB, 4);
		mOpencv.doProcess2(mRgba.getNativeObjAddr());
		Bitmap bmp = mBitmap;
		try {
			Utils.matToBitmap(mRgba, bmp);
		} catch (Exception e) {
			Log.e("org.opencv.samples.puzzle15",
					"Utils.matToBitmap() throws an exception: "
							+ e.getMessage());
			bmp.recycle();
			bmp = null;
		}

		return bmp;
	}


}
