#include <jni.h>
#include <iostream>
#include <string>
#include<Android/log.h>
#include <cmath>
#include <regex>
#include <unistd.h> // sleep 的头文件

#define  LOG_TAG  "muzi102"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);

using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    const char hell[] = "buttom";
    LOGD("Java_com_example_jnidemo_MainActivity_stringFromJNI");
    return env->NewStringUTF(hell);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_HelloJNI(JNIEnv *env, jobject thiz) {
    // TODO: implement HelloJNI()
    std::string hello = "这是我的第一个方法";
    int var1 = 20;
    int *ip_var1 = &var1;
    LOGD("结果%d", var1);
    LOGD("结果%d", ip_var1);
    LOGD("结果%d", *ip_var1);
    time_t now = time(0);
    char *dt = ctime(&now);
    LOGD("结果", now);
    return env->NewStringUTF(hello.c_str());
}

# define NUM_THREADS 5


void *say_hello(void *args) {
    LOGD("我是一个线程，我执行了！")
    return 0;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_threadTest(JNIEnv *env, jobject thiz) {
    std::string result = "执行完毕";
    pthread_t tids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        /**
         * 第一个参数 要地址 &
         * 第三个参数：需要执行的代码端，相当于runnable需要执行的
         */
        int ret = pthread_create(&tids[i], NULL, say_hello, NULL);
        if (ret != 0) {
            LOGD("pthread_create err : error code=%d", ret);
        }
    }
    return env->NewStringUTF(result.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_testNative(JNIEnv *env, jclass clazz, jstring msg) {
    // TODO: implement testNative()
    char const *str = "这个是C++的char";
    jstring jstr = env->NewStringUTF(str);
    const char *charstr = env->GetStringUTFChars(msg, 0);
    env->ReleaseStringUTFChars(msg, charstr);
    return jstr;
    /**
     * 获取最大的值
     */
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnidemo_MainActivity_getMaxNum(JNIEnv *env, jobject thiz, jintArray array) {
    // TODO: implement getMaxNum()
    //获取数组元素
    jint *arrayIp = env->GetIntArrayElements(array, 0);
    int len = env->GetArrayLength(array);
    jint result = 0;
    if (len > 0) {
        for (int i = 0; i < len; ++i) {
            jint item = arrayIp[i];
            LOGD("feifei item[%d]:%d", i, item)
            if (i == 0) {
                result = item;
            } else {
                //比较大小
                result = fmax(result, item);
            }
        }
    }
    jclass mClass = env->GetObjectClass(thiz);
    jmethodID mGetMaxNum = env->GetMethodID(mClass, "maxNum", "([I)I");
    jint max = env->CallIntMethod(thiz, mGetMaxNum, array);
    env->ReleaseIntArrayElements(array, arrayIp, 0);
    return max;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_sayName(JNIEnv *env, jobject thiz) {
    // TODO: implement sayName()
    jclass mClass = env->GetObjectClass(thiz);
    jmethodID sayName = env->GetMethodID(mClass, "sayName", "(Ljava/lang/String;)V");
    const char *name = "胡鹏";
    jstring mName = env->NewStringUTF(name);
    const char *nameIp = env->GetStringUTFChars(mName, 0);
    env->CallVoidMethod(thiz, sayName, mName);
    //释放
    env->ReleaseStringUTFChars(mName, nameIp);
}


#define STUDENT Lcom/example/jnidemo/Student

void *test(JNIEnv *pEnv, jobject pJobject);

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jnidemo_MainActivity_getNewStudio(JNIEnv *env, jobject thiz, jobject student) {
    // TODO: implement getNewStudio()
    jclass mClass = env->GetObjectClass(thiz);
    jclass studentd = env->FindClass("com/example/jnidemo/Student");
    jmethodID getAge = env->GetMethodID(studentd, "getAge", "()I");
    jstring mName = env->NewStringUTF("胡鹏");
    char const *nameip = env->GetStringUTFChars(mName, 0);
    env->CallIntMethod(student, getAge);
    return student;
}


struct FunPara {
    JNIEnv *e;
    jobject jobject1;
};


void *test(void *args) {
    FunPara *para;
    para = (FunPara *) args;
    for (int i = 0; i < 1000; ++i) {
        LOGD("第%d本书", i);
//        publishJavaProgress(para->e, para->jobject1, i);
        usleep(1000);
    }
    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_startThread(JNIEnv *env, jobject thiz) {
    pthread_t pthread;
    struct FunPara para;
    para.e = env;
    para.jobject1 = thiz;
//    struct FunPara *p;
//    p->e = env;
//    p->jobject1 = thiz;
    int result = pthread_create(&pthread, NULL, test, &para);
    if (result != 0) {
        LOGD("pthread_create err : error code=%d", result);
    } else {
        LOGD("线程创建成功");
    }
}


void onFail(JNIEnv *env, jobject on_listener, char *fail) {
    if (on_listener != NULL) {
        jclass onListent = env->FindClass("com/example/jnidemo/MainOnListener");
        jmethodID onCallBace = env->GetMethodID(onListent, "onFail", "(Ljava/lang/String;)V");
        jstring str = env->NewStringUTF(fail);
        env->CallVoidMethod(on_listener, onCallBace, str);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_onListener(JNIEnv *env, jobject thiz, jobject on_listener) {
    jclass mainActivity = env->GetObjectClass(thiz);
    if (on_listener != NULL) {
        jclass onListent = env->FindClass("com/example/jnidemo/MainOnListener");
        jmethodID onCallBace = env->GetMethodID(onListent, "onCallBace", "(I)V");
        env->CallVoidMethod(on_listener, onCallBace, 5);
    }
    char *str = "data is null";
    onFail(env, on_listener, str);
}

