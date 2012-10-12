package com.example.opencvtest;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.provider.MediaStore.Images.Media;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;
import android.widget.Toast;

@SuppressLint("NewApi")
public class IndoorNavigationActivity extends Activity {
	private static final String IMAGE_DIRECTORY = "/sdcard/DCIM/Camera";
	private static final int ACTIVITY_SELECT_CAMERA = 0;
	private static final int ACTIVITY_SELECT_IMAGE = 1;
	private static final String TAG = "MAIN_ACTIVITY";
	private static final int CAMERA_ID = Menu.FIRST;
	private static final int GALLERY_ID = Menu.FIRST + 1;
	private String mCurrentImagePath = null;
	private OpenCV opencv = new OpenCV();
	//private TestCV testCV= new TestCV();
	private ImageView mImageView;
	private ProgressDialog progressDialog;
	MyAsyncTask aTask = new MyAsyncTask();
	Bitmap mBitmap;
	private Uri fileUri;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		mImageView = new ImageView(this);
		// setContentView(R.layout.activity_camera);
		
		setContentView(mImageView);
	
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		menu.add(0, CAMERA_ID, 0, "Camera");
		menu.add(0, GALLERY_ID, 0, "Gallery");
		return true;
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
		case CAMERA_ID:
			Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			long timeTaken = System.currentTimeMillis();
			mCurrentImagePath = IMAGE_DIRECTORY + "/"
					+ Utility.createName(timeTaken) + ".jpg";
			Log.i(TAG, mCurrentImagePath);
			//fileUri=Uri.fromFile(new File(mCurrentImagePath));
			cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT,Uri.fromFile(new File(mCurrentImagePath)));
			startActivityForResult(cameraIntent, ACTIVITY_SELECT_CAMERA);
			
			//Intent cameraIntent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE); 
            //startActivityForResult(cameraIntent, 1888); 
          return true;
		case GALLERY_ID:
			Intent galleryIntent = new Intent(Intent.ACTION_PICK,
					Images.Media.INTERNAL_CONTENT_URI);
			startActivityForResult(galleryIntent, ACTIVITY_SELECT_IMAGE);
			//runDialog(100);
			return true;
		}

		return super.onMenuItemSelected(featureId, item);
	}
	
	public Bitmap getResizedBitmap(Bitmap bm, int newHeight, int newWidth) {
        int width = bm.getWidth();
        int height = bm.getHeight();
        float scaleWidth = ((float) newWidth) / width;
        float scaleHeight = ((float) newHeight) / height;
        // CREATE A MATRIX FOR THE MANIPULATION
        Matrix matrix = new Matrix();
        // RESIZE THE BIT MAP
        matrix.postScale(scaleWidth, scaleHeight);


        // RECREATE THE NEW BITMAP
        Bitmap resizedBitmap = Bitmap.createBitmap(bm, 0, 0, width, height, matrix, false);
        return resizedBitmap;
    }
	
	public void extractFeature(Bitmap mBitmap)
	{
		
		mBitmap= getResizedBitmap(mBitmap, 480, 640);
		
		
		Log.i("BitmapSize", "Size"+mBitmap.getRowBytes());
		
		int width = mBitmap.getWidth();
		int height = mBitmap.getHeight();
		
		int[] pixels = new int[width * height];
		mBitmap.getPixels(pixels, 0, width, 0, 0, width, height);
		//testCV.setSourceImage(pixels, width, height);
		opencv.setSourceImage(pixels, width, height);
		long start = System.currentTimeMillis();
		//opencv.extractSURFFeature();
		opencv.findEdgesandCorners();
		opencv.getCornerpoints();
		long end = System.currentTimeMillis();
		//byte[] imageData = testCV.getSourceImage();
		byte[] imageData = opencv.getSourceImage();
		long elapse = end - start;
		Toast.makeText(this, "" + elapse + " ms is used to extract features.",
				Toast.LENGTH_LONG).show();
		mBitmap = BitmapFactory.decodeByteArray(imageData, 0,
				imageData.length);
		mImageView.setImageBitmap(mBitmap);

	}

	
	void showDialog()
	{
		
		//progressDialog = ProgressDialog.show(this, "Please wait....", "Image Processing");
		progressDialog = new ProgressDialog(this);
		progressDialog.setTitle("Please wait....");
		progressDialog.setMessage( "Image Processing");       
		progressDialog.setIndeterminate(false);
		progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
		progressDialog.show();
	}
	
	void dismissDialog()
	{
		progressDialog.dismiss();
	}
	
	
	class MyAsyncTask extends AsyncTask<Integer, Integer, Long> {
        
        @Override
        protected Long doInBackground(Integer... params) {
        	progressDialog.setProgress(0);  
            long start = System.currentTimeMillis();
			mBitmap = BitmapFactory.decodeFile(mCurrentImagePath);
			
			mBitmap= getResizedBitmap(mBitmap, 480, 640);

			Log.i("BitmapSize", "Size"+mBitmap.getRowBytes());
			
			int width = mBitmap.getWidth();
			int height = mBitmap.getHeight();
			
			int[] pixels = new int[width * height];
			mBitmap.getPixels(pixels, 0, width, 0, 0, width, height);
			publishProgress(10);
			//testCV.setSourceImage(pixels, width, height);
			opencv.setSourceImage(pixels, width, height);
			publishProgress(10);
			//opencv.extractSURFFeature();
			opencv.findEdgesandCorners();
			publishProgress(25);
			opencv.getCornerpoints();
			
			publishProgress(35);
			long end = System.currentTimeMillis();
			//byte[] imageData = testCV.getSourceImage();
			
			byte[] imageData = opencv.getSourceImage();
			publishProgress(10);
			long elapse = end - start;
/*			Toast.makeText(this, "" + elapse + " ms is used to extract features.",
					Toast.LENGTH_LONG).show();*/
			mBitmap = BitmapFactory.decodeByteArray(imageData, 0,
					imageData.length);
			publishProgress(10);
			
            return start - System.currentTimeMillis();
        }
 
         
         
        @Override
        protected void onProgressUpdate(Integer... values) {
        	progressDialog.incrementProgressBy((int)(values[0]));
        }
         
        @Override
        protected void onPostExecute(Long time) {
            //updateUI("Done with all the operations, it took:" +
                                  //   time + " millisecondes");
        	mImageView.setImageBitmap(mBitmap);
        	dismissDialog();
        	aTask = new MyAsyncTask();
        }
 
        @Override
        protected void onPreExecute() {
        	showDialog();
        	//progressDialog = ProgressDialog.show(getApplicationContext(), "Please wait....", "Here your message");
            //updateUI("Starting process");
        }
         
         
        public void doLongOperation() {
             
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
             
        }
         
    }

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) 
	{

		if (requestCode == ACTIVITY_SELECT_CAMERA
				&& resultCode == Activity.RESULT_OK) {
			aTask.execute(1);
			/*ContentValues values = new ContentValues();
			int degrees = Utility.getRotationFromImage(mCurrentImagePath);
			try {
				ExifInterface exif = new ExifInterface(mCurrentImagePath);
				float[] position = new float[2];
				if (exif.getLatLong(position)) {
					values.put(Images.Media.LATITUDE, position[0]);
					values.put(Images.Media.LONGITUDE, position[1]);
				}
			} catch (Exception e) {

			}
			// reduce the size of image
			try 
			{
				BitmapFactory.Options option = new BitmapFactory.Options();
				option.inSampleSize = 4;
				
				Bitmap bitmap = BitmapFactory.decodeFile(mCurrentImagePath,
						option);
				if (degrees != 0) {
					bitmap = Utility.rotate(bitmap, degrees);
				}
				FileOutputStream out = new FileOutputStream(mCurrentImagePath);
				bitmap.compress(CompressFormat.JPEG, 50, out);
				mImageView.setImageBitmap(bitmap);
			} catch (Exception e) {

			}
			
			values.put(Images.Media.MIME_TYPE, "image/jpeg");
			values.put(Images.Media.DATA, mCurrentImagePath);
			values.put(Images.Media.ORIENTATION, degrees);
			getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values);*/
			
			
		}
		if (requestCode == ACTIVITY_SELECT_IMAGE && resultCode == RESULT_OK) 
		{
			//progressDialog = ProgressDialog.show(getApplicationContext(), "Please wait....", "Here your message");
			 Uri currImageURI = data.getData();
				String[] proj = { Images.Media.DATA, Images.Media.ORIENTATION };
				Cursor cursor = managedQuery(currImageURI, proj, null, null,
						null);
				int columnIndex = cursor.getColumnIndex(proj[0]);
				cursor.moveToFirst();
				mCurrentImagePath = cursor.getString(columnIndex);
			aTask.execute(1);
			//progressDialog.dismiss();
			
		}
	}
}