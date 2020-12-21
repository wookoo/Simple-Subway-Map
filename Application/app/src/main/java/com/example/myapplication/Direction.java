package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.ArrayList;

public class Direction extends AppCompatActivity {
    LinearLayout mContainer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_direction);
        Intent Data = getIntent();
        ArrayList<String> Stations = Data.getStringArrayListExtra("Stations");
        ArrayList<Integer> Times = Data.getIntegerArrayListExtra("Times");
        int TotalTime = Data.getIntExtra("TotalTime",0);
        Log.d("데이터 받아온거",Stations.toString());
        TextView mStartView = findViewById(R.id.directionStartText);
        TextView mEndView = findViewById(R.id.directionEndText);
        mStartView.setText(Stations.get(0));
        mEndView.setText(Stations.get(Stations.size()-1));
        mContainer = findViewById(R.id.LineCon);
        makeTextView("총 소요 시간 : " + TotalTime +" 분 ");
        makeTextView("============");
        for(int i = 0;i<Stations.size()-1;i++){
            makeTextView(Stations.get(i));
            makeTextView("|\n"+Times.get(i)+" 분 소요\n|");
        }
        makeTextView(Stations.get(Stations.size()-1));
        makeTextView("============");



    }
    private void makeTextView(String text){
        TextView t1 = new TextView(this);
        t1.setText(text);
        t1.setTextColor(Color.BLACK);
        t1.setTextSize(15);
        t1.setGravity(Gravity.CENTER);
        mContainer.addView(t1);
    }
}