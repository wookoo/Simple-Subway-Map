package com.example.myapplication;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.regex.Pattern;

public class LineInterface {
    WebView mAppView;
    static Activity mContext;
    String start = null;
    String end = null;
    TextView startView;
    TextView endView;


    public LineInterface(Activity activity, WebView view,TextView startView,TextView endView) {
        this.mAppView = view;
        this.mContext = activity;
        this.startView = startView;
        this.endView = endView;

    }

    @JavascriptInterface
    public void showSubwayInfo(String station) {
       Log.d("역 눌림",station);
        AlertDialog.Builder dialog = new AlertDialog.Builder(mContext);
        dialog.setTitle(station+"역의 작업을 선택해주세요");

        dialog.setPositiveButton("출발지", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Log.d("출발지", station);

                        if(station.equals(end)){
                            Toast.makeText(mContext,"출발지와 도착지는 같을수 없습니다 ! 출발지를 다시 설정하세요",Toast.LENGTH_SHORT).show();//오류출력
                        }
                        else{
                            start = station;
                            startView.setText(station);
                            getData("http://172.30.1.31:5000/message");
                        }

                    }
                });
        dialog.setNegativeButton("도착지", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Log.d("도착지", station);
                if(station.equals(start)){
                    Toast.makeText(mContext,"출발지와 도착지는 같을수 없습니다 ! 도착지를 다시 설정하세요",Toast.LENGTH_SHORT).show(); //오류출력

                }
                else{
                    end = station;
                    endView.setText(station);
                    getData("http://172.30.1.31:5000/message");
                }
            }
        });
        dialog.setNeutralButton("취소", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Log.d("취소", station);
            }
        });

        dialog.show();
    }

    private void getData(final String urls) {
        if (this.start == null || this.end == null){ //시작정점이나 도착정점이 없으면 종료
            return;
        }
        new Thread() {
            @Override
            public void run() {
                try {

                    URL url = new URL(urls);
                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                    connection.setRequestMethod("POST"); //전송방식
                    connection.setDoOutput(true);       //데이터를 쓸 지 설정
                    connection.setDoInput(true);        //데이터를 읽어올지 설정

                    OutputStream os = connection.getOutputStream();
                    JSONObject sendData = new JSONObject(); //Json 오브젝트 생성
                    sendData.put("start",end); //서버에 보낼 정보
                    sendData.put("end",start);
                    start = null; //서버에 보낼 데이터 설정했으므로 start end 초기화
                    end = null;
                    startView.setText("");
                    endView.setText("");
                    //sendData.put("start", "서울");
                    //sendData.put("end", "공덕");
                    String body = sendData.toString();//requests 에 body 세팅
                    os.write(body.getBytes("utf-8")); //전송할 데이터 설정

                    // Request Body에 Data 입력.
                    os.flush();

                    // OutputStream 종료.
                    os.close();

                    InputStream is = connection.getInputStream();
                    StringBuilder sb = new StringBuilder();
                    BufferedReader br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
                    String result; //결과값
                    while ((result = br.readLine()) != null) {
                        sb.append(result + "\n"); //결과값 버퍼 추가
                    }

                    connection.disconnect(); //연결 종료

                    result = sb.toString(); //받아온 데이터


                    JSONObject obj = new JSONObject(result);
                    String resultValue = obj.getString("result");
                    String[] Direct = resultValue.split("\n")[1].split(" - ");// 2번째 라인을 -로 쪼개서 숫자 가져옴
                    Log.d("서버에서 받아온 값", resultValue);//받아온 데이터는 result키의 value 임!
                    ArrayList<String> Stations = new ArrayList<String>(); //경로정보
                    ArrayList<Integer> Times = new ArrayList<Integer>(); //소요시간 정보
                    int TotalTime = 0; //총 시간 다시 구하기 위한 변수
                    for(int i = 0; i <Direct.length;i++){
                        if(i%2==0){ //역명
                            Stations.add(Direct[i]);
                        }
                        else{
                            int weight = Integer.parseInt(Direct[i].replace("(","").replace(")",""))/60; //이동시간은 초단위임으로 60으로 나눔
                            TotalTime += weight;
                            Times.add(weight);
                        }
                    }


                    Intent showDirectoryIntent = new Intent(mContext,Direction.class);
                    showDirectoryIntent.putExtra("TotalTime",TotalTime);
                    showDirectoryIntent.putExtra("Stations",Stations);
                    showDirectoryIntent.putExtra("Times",Times);
                    mContext.startActivity(showDirectoryIntent); //최단거리 사용자에게 보여주는 코드


                } catch (Exception e) {
                    Log.d("error",e.toString());
                }

            }
        }.start();
    }
}