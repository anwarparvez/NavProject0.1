package com.example.opencvtest;

import java.util.ArrayList;
import java.util.HashMap;

import android.app.Activity;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class MyListAdapter extends BaseAdapter {
    private Activity activity;
    private static LayoutInflater inflater=null;
	String[] presidents={"image","video","camera"};
	int []image_id={R.drawable.content_picture,R.drawable.ic_action_video,R.drawable.device_access_camera};
	public MyListAdapter(Activity activity) {
		this.activity=activity;
		 // presidents = context.getResources().getStringArray(R.array.presidents_array);
	       inflater = (LayoutInflater)activity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
  //  public ImageLoader imageLoader; 
	public int getCount() {
		// TODO Auto-generated method stub
		return presidents.length;
	}

	public Object getItem(int position){
		// TODO Auto-generated method stub
		return position;
	}

	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
	        View vi=convertView;
	        if(convertView==null)
	            vi = inflater.inflate(R.layout.list_row, null);
	 
	        TextView title = (TextView)vi.findViewById(R.id.title_song); // title
	        ImageView imageView = (ImageView)vi.findViewById(R.id.list_image); // title
	        imageView.setImageResource(image_id[position]);
	        // Setting all values in listview
	        title.setText(presidents[position].toUpperCase());
	        

	       // imageLoader.DisplayImage(presidents[position]);
	        return vi;
	}
}
