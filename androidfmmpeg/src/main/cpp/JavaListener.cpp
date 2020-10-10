//
// Created by 15656 on 2020/10/10.
//
#include "JavaListener.h"

JavaListener::JavaListener(JavaVM *vm, JNIEnv *env, jobject jobject2) {
    jvm = vm;
    this->env = env;
    this->jobject1 = jobject2;
    jclass clzz = env->GetObjectClass(jobject2);
    if (clzz == nullptr) {
        return;
    }
    jmid = env->GetMethodID(clzz, "onErrorCallBack", "(ILjava/lang/String;)V");
}


void JavaListener::onError(int type, int code, const char *msg) {
    if (type == 0) {
        //主线程中
        jstring jstring1 = env->NewStringUTF(msg);
        env->CallVoidMethod(jobject1, jmid, code, jstring1);
        env->DeleteLocalRef(jstring1);
    } else {
        //子线程中
        JNIEnv *env1;
        //通过VM获取当前主线程的env
        jvm->AttachCurrentThread(&env1, 0);
        jstring jstring1 = env1->NewStringUTF(msg);
        env1->CallVoidMethod(jobject1, jmid, code, jstring1);
        env1->DeleteLocalRef(jstring1);
        jvm->DetachCurrentThread();
    }
}


JavaListener::~JavaListener() {

}