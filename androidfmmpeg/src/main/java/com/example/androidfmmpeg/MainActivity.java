package com.example.androidfmmpeg;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.io.File;


public class MainActivity extends AppCompatActivity implements NativeDemo.OnErrorListener {
    private NativeDemo nativeDemo;
    String musicPath;
    static int REQUEST_EXTERNAL_STORAGE = 102;

    private static String[] PERMISSIONS_STORAGE = {
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        musicPath = "/sdcard/music/mydream.pcm";
        File file = new File(musicPath);
        if (file.isFile()) {
            Log.d("muzi102", "文件存在");
        } else {
            Log.d("muzi102", "文件不存在");
        }
        nativeDemo = new NativeDemo();
        nativeDemo.setOnErrorListener(this);
        verifyStoragePermissions(this);
        Log.d("muzi102", file.getAbsolutePath());

    }


    //线程
    public void textThread(View view) {
        nativeDemo.n_textThread();
    }

    //生产者消费者
    public void testMutexThread(View view) {
        nativeDemo.n_textMutexThread();
    }

    //C++调用Jave实现回调
    public void testCallBackFromC(View view) {
        nativeDemo.onErrorCallBackFromC();
    }

    //OpenSL ES 进行PCM音频文件播放
    public void OpenSLES(View view) {
        nativeDemo.n_testOpenSLES(musicPath);
    }

    @Override
    public void onError(int code, String msg) {
        Log.d("muzi102", "MainActivity 收到回调信息 code=" + code + " msg=" + msg);
    }


    //然后通过一个函数来申请
    public static void verifyStoragePermissions(Activity activity) {
        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(activity,
                    "android.permission.WRITE_EXTERNAL_STORAGE");
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
