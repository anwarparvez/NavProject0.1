package com.samsung.indoornavigation.fragment;

import java.io.FileNotFoundException;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.MediaStore.Images;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.samsung.indoornavigation.R;
import com.samsung.indoornavigation.opencv.OpenCV;

public class ActionDetailFragment extends Fragment {
	protected static final int ACTIVITY_SELECT_IMAGE = 10;
	View mContentView = null;
	ProgressBar bar;
	MyAsyncTask mAsyncTask;
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
						Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
						intent.setType("image/*");
						startActivityForResult(intent, ACTIVITY_SELECT_IMAGE);

					}
				});
		return mContentView;
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == ACTIVITY_SELECT_IMAGE
				&& resultCode == Activity.RESULT_OK) {

			Uri currImageURI = data.getData();
			/*
			 * Toast.makeText(getActivity(), " " + currImageURI,
			 * Toast.LENGTH_LONG) .show();
			 */

			try {
				mBitmap = BitmapFactory.decodeStream(getActivity()
						.getContentResolver().openInputStream(currImageURI));
				ImageView targetImage = (ImageView) mContentView
						.findViewById(R.id.imageView1);
				if (mBitmap != null && targetImage != null) {

					String[] proj = { Images.Media.DATA,
							Images.Media.ORIENTATION };
					Cursor cursor = getActivity().managedQuery(currImageURI,
							proj, null, null, null);
					int columnIndex = cursor.getColumnIndex(proj[0]);
					cursor.moveToFirst();
					mCurrentImagePath = cursor.getString(columnIndex);
					/*
					 * Toast.makeText(getActivity(), " " + mCurrentImagePath,
					 * Toast.LENGTH_LONG).show();
					 */
					Log.i("TagMe", mCurrentImagePath.toString() + "\n");
					// targetImage.setImageBitmap(mBitmap);
					mAsyncTask = new MyAsyncTask();
					mAsyncTask.execute(0);
				}
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

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

	class MyAsyncTask extends AsyncTask<Integer, Integer, Long> {

		public MyAsyncTask(Activity context) {

		}

		public MyAsyncTask() {
			// TODO Auto-generated constructor stub
		}

		@Override
		protected Long doInBackground(Integer... params) {

			long start = System.currentTimeMillis();
			// int width = mBitmap.getWidth();
			// int height = mBitmap.getHeight();
			// int[] pixels = new int[width * height];
			// mBitmap.getPixels(pixels, 0, width, 0, 0, width, height);
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
