package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.webkit.WebView;
import android.widget.TextView;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    private LineInterface lineInterface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        WebView lineViewer = findViewById(R.id.lineViewer);
        TextView startView = findViewById(R.id.startTextView);
        TextView endView = findViewById(R.id.endTextView);
        lineInterface = new LineInterface(this,lineViewer,startView,endView);
        lineViewer.getSettings().setJavaScriptEnabled(true);
        lineViewer.getSettings().setBuiltInZoomControls(true);
        lineViewer.getSettings().setSupportZoom(true);
        lineViewer.getSettings().setDisplayZoomControls(false);
        lineViewer.getSettings().setDefaultTextEncodingName("UTF-8");
        lineViewer.addJavascriptInterface(lineInterface,"Android");
        lineViewer.loadUrl("file:///android_asset/mSeoul_Subway.html");

    }


}