//
// Created by 15656 on 2020/10/10.
//
#include "jni.h"

#ifndef JNIDEMO_JAVALISTENER_H
#define JNIDEMO_JAVALISTENER_H


class JavaListener {

public:
    JavaVM *jvm;
    jobject jobject1;
    jmethodID jmid;
    JNIEnv *env;


public:
    JavaListener(JavaVM *jvm, JNIEnv *jenv,jobject jobject1);
    /**
     * type:0 表示在主线程 1 表示在子线程
     * @param type
     * @param code
     * @param msg
     */
    void onError(int type,int code, const char *msg);

    ~JavaListener();
};


#endif //JNIDEMO_JAVALISTENER_H
