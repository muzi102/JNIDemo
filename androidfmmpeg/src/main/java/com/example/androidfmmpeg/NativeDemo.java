package com.example.androidfmmpeg;

/**
 * @author :muzi102
 * @time : 2020/10/9
 * @Description
 */
public class NativeDemo {


    private OnErrorListener onErrorListener;

    static {
        System.loadLibrary("muzi-lib");
    }

    public native void n_textThread();
    public native void n_textMutexThread();
    public native void onErrorCallBackFromC();



    /**
     * 错误信息回调
     *
     * @param code
     * @param msg
     */
    public void onErrorCallBack(int code, String msg) {
        if (onErrorListener != null) {
            onErrorListener.onError(code, msg);
        }
    }



    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }

    public interface OnErrorListener {
        void onError(int code, String msg);
    }
}
