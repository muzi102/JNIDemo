package com.example.jnidemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.TextureView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.List;

public class MainActivity extends AppCompatActivity implements MainOnListener {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String HelloJNI();

    public native String threadTest();

    public native String testMuZi();

    public native static String testNative(String msg);


    public native Student getNewStudio(Student student);

    /**
     * 获取数组中最大的值
     *
     * @return
     */
    public native int getMaxNum(int[] array);

    public native void sayName();


    public void setConvertProgress(int progress) {

        Log.e("progress", "当前进度为=" + progress);
    }

    public void onClick(View view) {
//        HelloJNI();
//        int result = getMaxNum(new int[]{54, 5, 6, 6, 26, 14, 942, 100});
////        tomas("结果是 =" + result);
//        sayName();
//        getNewStudio(new Student("", 0));
//        startThread();
        onListener(this);
    }


    /**
     * 获取list中最大的值
     *
     * @param arry
     * @return
     */
    public int maxNum(int[] arry) {
        Integer result = null;
        if (arry != null && arry.length > 0) {
            for (Integer integer : arry) {
                if (result != null) {
                    if (integer > result) {
                        result = integer;
                    }
                } else {
                    result = integer;
                }
            }
        }
        return result;
    }

    public void sayName(String name) {
        tomas("我的名字是：" + name);
    }

    private void tomas(String content) {
        Toast.makeText(MainActivity.this, content, Toast.LENGTH_LONG).show();
    }

    public void printStudent(Student student) {
        tomas(student.toString());
    }

    public native void startThread();


    public native void onListener(MainOnListener onListener);

    @Override
    public void onCallBace(int result) {
        Log.e("onCallBace", "我收到的是 result = " + result);
    }

    @Override
    public void onFail(String fail) {
        tomas("错误信息：" + fail);
    }

}
