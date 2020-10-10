//
// Created by 15656 on 2020/8/4.
//
#include <jni.h>
#include <string>

#include<Android/log.h>

#define  LOG_TAG  "muzi"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_testMuZi(JNIEnv *env, jobject thiz) {
    // TODO: implement testMuZi()

    const char hell[] = "这是一个C++的字符串";
    const char secondStr[] = "第二个字符串";
    int result = strcmp(hell, secondStr);
    std::string content = "这是一个非常好的开始";

//    int numb = 100.54;

    time_t now = time(0);
    char *dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    return env->NewStringUTF(hell);
}


