//
// Created by 15656 on 2020/10/6.
//
#include <jni.h>
#include<Android/log.h>
#include<string>
#include <unistd.h>
#include "pthread.h"
#include "queue"
#include "JavaListener.h"

#define  LOG_TAG  "muzi102"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);


void *threadwork(void *arg) {
    LOGD("执行线程工作");
    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidfmmpeg_NativeDemo_n_1textThread(JNIEnv *env, jobject thiz) {
    // TODO: implement n_textThread()
    pthread_t mThread;
    int result = pthread_create(&mThread, NULL, threadwork, NULL);
    if (result != 0) {
        LOGD("线程异常");
    } else {
        LOGD("线程启动成功");
    }
}


pthread_t produc;
pthread_t custom;

pthread_mutex_t mutex;
pthread_cond_t cond;
std::queue<int> queue;
bool isExit = false;

void *producCallBack(void *arg);

void *customCallBack(void *arg);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidfmmpeg_NativeDemo_n_1textMutexThread(JNIEnv *env, jobject thiz) {
    // TODO: implement n_textMutexThread()
    for (int i = 0; i < 10; ++i) {
        queue.push(i);
        LOGD("生产者生产一个产品 %d", i)
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&produc, NULL, producCallBack, NULL);
    pthread_create(&custom, NULL, customCallBack, NULL);
}

void *producCallBack(void *arg) {
    while (!isExit) {
        //加入数据 线程加锁
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOGD("生产者生产一个产品，通知消费者")
        //通知消费者
        pthread_cond_signal(&cond);
        //线程解锁
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }
    pthread_exit(&produc);
}

void *customCallBack(void *arg) {
    //pop数据
    while (!isExit) {
        pthread_mutex_lock(&mutex);
        if (queue.size() > 0) {
            queue.pop();
            LOGD("消费者消费产品，剩余产品数量=%d", queue.size())
        } else {
            LOGD("没有产品可以消费，等待中……")
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000 * 500);
    }
    pthread_exit(&custom);
}


JavaVM *jvm;
JavaListener *javaListener;
pthread_t testOnErrorThread;

void *onErrorCallBace(void *arg) {
    sleep(5);
    JavaListener *jlister = (JavaListener *) arg;
    jlister->onError(1, 103, "C++ call java meid from child thread");
    pthread_exit(&testOnErrorThread);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidfmmpeg_NativeDemo_onErrorCallBackFromC(JNIEnv *env, jobject thiz) {
    // TODO: implement onErrorCallBackFromC()
    //env->NewGlobalRef(thiz) 变成全局的object
    javaListener = new JavaListener(jvm, env, env->NewGlobalRef(thiz));
    javaListener->onError(0, 102, "C++ call  java meid from main thread ");
    pthread_create(&testOnErrorThread, nullptr, onErrorCallBace, javaListener);
}


/**
 * 保存vm和JNI版本
 * @param vm
 * @param reserved
 * @return
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    jvm = vm;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}