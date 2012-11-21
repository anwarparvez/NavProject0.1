package net.learn2develop.Fragments;

import com.example.opencvtest.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Fragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.TextView;


public class Fragment2 extends Fragment {
	Activity mActivity;
	private static final String TAG = "Sample::Activity";
	private Sample3View mView;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// ---Inflate the layout for this fragment---
		return inflater.inflate(R.layout.fragment2, container, false);
	}

	@Override
	public void onStart() {
		super.onStart();
		FrameLayout fl = (FrameLayout) getActivity()
				.findViewById(R.id.fdetails);
		mView = new Sample3View(getActivity());
		TextView textView=new TextView(getActivity());
		fl.addView(mView );
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		FrameLayout fl = (FrameLayout) getActivity()
				.findViewById(R.id.fdetails);
		//mView = new Sample3View(getActivity());
		//TextView textView=new TextView(getActivity());
	//	fl.addView(textView);
		

		// Create and set View

	}

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		mActivity = activity;

		Log.d("Fragment 1", "onAttach");
	}

	@Override
	public void onPause() {
		Log.i(TAG, "onPause");
		super.onPause();
		if (null != mView)
			mView.releaseCamera();
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if ((null != mView) && !mView.openCamera()) {
			AlertDialog ad = new AlertDialog.Builder(mActivity).create();
			ad.setCancelable(false); // This blocks the 'BACK' button
			ad.setMessage("Fatal error: can't open camera!");
			ad.setButton("OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					dialog.dismiss();
					mActivity.finish();
				}
			});
			ad.show();
		}
	}

}
