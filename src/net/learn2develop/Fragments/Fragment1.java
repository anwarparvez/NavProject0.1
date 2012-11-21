package net.learn2develop.Fragments;

import com.example.opencvtest.R;

import android.app.Activity;
import android.app.Fragment;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

public class Fragment1 extends Fragment {
	ImageView mImageView;

	public void showImage(Bitmap mBitmap) {
		mImageView.setImageBitmap(mBitmap);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		Log.d("Fragment 1", "onCreateView");

		// ---Inflate the layout for this fragment---
		return inflater.inflate(R.layout.fragment1, container, false);
	}

	@Override
	public void onStart() {
		super.onStart();
		mImageView = (ImageView) getActivity().findViewById(R.id.imageView1);
		Log.d("Fragment 1", "onStart");
	}

}
