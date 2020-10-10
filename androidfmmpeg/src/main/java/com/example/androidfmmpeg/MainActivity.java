package com.example.androidfmmpeg;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;


public class MainActivity extends AppCompatActivity implements NativeDemo.OnErrorListener {
    private NativeDemo nativeDemo;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        nativeDemo = new NativeDemo();
        nativeDemo.setOnErrorListener(this);
    }


    //线程
    public void textThread(View view) {
        nativeDemo.n_textThread();
    }


    public void testMutexThread(View view) {
        nativeDemo.n_textMutexThread();
    }


    public void testCallBackFromC(View view) {
        nativeDemo.onErrorCallBackFromC();
    }

    @Override
    public void onError(int code, String msg) {
        Log.d("muzi102", "MainActivity 收到回调信息 code=" + code + " msg=" + msg);
    }
}
