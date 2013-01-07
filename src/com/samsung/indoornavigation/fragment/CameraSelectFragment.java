package com.samsung.indoornavigation.fragment;

import java.io.File;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.samsung.indoornavigation.R;
import com.samsung.indoornavigation.opencv.OpenCV;
import com.samsung.indoornavigation.opencv.Utility;

public class CameraSelectFragment extends Fragment {

	private static final String IMAGE_DIRECTORY = "/sdcard/DCIM/Camera";
	private static final int ACTIVITY_SELECT_CAMERA = 20;
	private static final int ACTIVITY_SELECT_IMAGE = 10;
	private static final String TAG = "MAIN_ACTIVITY";
	View mContentView = null;
	ProgressBar bar;
	MyAsyncTask2 mAsyncTask;
	String mCurrentImagePath;
	public OpenCV mOpencv = new OpenCV();
	Bitmap mBitmap;

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onActivityCreated(savedInstanceState);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mContentView = inflater.inflate(R.layout.action_detail, null);
		bar = (ProgressBar) mContentView.findViewById(R.id.progressBar1);

		mContentView.findViewById(R.id.select_btn).setOnClickListener(
				new View.OnClickListener() {

					public void onClick(View v) {
/*						Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
						intent.setType("image/*");
						startActivityForResult(intent, ACTIVITY_SELECT_IMAGE);*/
						
						Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
						long timeTaken = System.currentTimeMillis();
						mCurrentImagePath = IMAGE_DIRECTORY + "/"
								+ Utility.createName(timeTaken) + ".jpg";
						//Log.i(TAG, mCurrentImagePath);
						// fileUri=Uri.fromFile(new File(mCurrentImagePath));
						cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT,
								Uri.fromFile(new File(mCurrentImagePath)));
						startActivityForResult(cameraIntent, ACTIVITY_SELECT_CAMERA);


					}
				});
		return mContentView;
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == ACTIVITY_SELECT_CAMERA
				&& resultCode == Activity.RESULT_OK) {
			Log.i("TagMe", mCurrentImagePath.toString() + "\n");
			// targetImage.setImageBitmap(mBitmap);
			mAsyncTask = new MyAsyncTask2();
				mAsyncTask.execute(0);
		}
	}

	public void showDetails(String device) {

		Toast.makeText(getActivity(), " " + device, Toast.LENGTH_LONG).show();
		/*
		 * this.device = device; this.getView().setVisibility(View.VISIBLE);
		 * TextView view = (TextView)
		 * mContentView.findViewById(R.id.device_address);
		 * view.setText(device.deviceAddress); view = (TextView)
		 * mContentView.findViewById(R.id.device_info);
		 * view.setText(device.toString());
		 */

	}

	class MyAsyncTask2 extends AsyncTask<Integer, Integer, Long> {

		public MyAsyncTask2(Activity context) {

		}

		public MyAsyncTask2() {
			// TODO Auto-generated constructor stub
		}

		@Override
		protected Long doInBackground(Integer... params) {

			long start = System.currentTimeMillis();
			// int width = mBitmap.getWidth();
			// int height = mBitmap.getHeight();
			// int[] pixels = new int[width * height];
			// mBitmap.getPixels(pixels, 0, width, 0, 0, width, height);
			if(mCurrentImagePath==null)
			{
				return null;
			}
			publishProgress(10);
			mOpencv.setSourceImage(null, 0, 0, mCurrentImagePath);
			publishProgress(10);

			mOpencv.findEdgesandCorners();
			publishProgress(10);
			mOpencv.getCornerpoints();
			publishProgress(10);
			byte[] imageData = mOpencv.getSourceImage();
			publishProgress(10);

			/*
			 * Toast.makeText(this, "" + elapse +
			 * " ms is used to extract features.", Toast.LENGTH_LONG).show();
			 */
			mBitmap = BitmapFactory.decodeByteArray(imageData, 0,
					imageData.length);

			long end = System.currentTimeMillis();
			long elapse = end - start;
			return elapse;
		}

		@Override
		protected void onProgressUpdate(Integer... values) {
			bar.incrementProgressBy(values[0]);
		}

		@Override
		protected void onPostExecute(Long time) {
			bar.setProgress(0);
			ImageView targetImage = (ImageView) mContentView
					.findViewById(R.id.imageView1);
			targetImage.setImageBitmap(mBitmap);
		}

		@Override
		protected void onPreExecute() {

			bar.setProgress(0);

		}

	}

}
