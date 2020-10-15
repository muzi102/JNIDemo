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

#include "SLES/OpenSLES_Android.h"
#include "SLES/OpenSLES.h"
#include<Android/log.h>
#include <cerrno>

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

/**
 * 1 创建接口对象
 * 2 设置混音器
 * 3.创建播放器（录音器）
 * 4 设置缓冲队列和回调函数
 * 5 设置播放状态
 * 6 启动回调函数
 */

//309219417906
//引擎接口
SLObjectItf engineObject = nullptr;
SLEngineItf engineEngine = nullptr;
//混音器
SLObjectItf outputMixObject = nullptr;
SLEnvironmentalReverbItf outputMixEnvironmentalReverbItf = nullptr;
SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
//pcm
SLObjectItf pcmPlayerObject = nullptr;
SLPlayItf pcmPlayerPlay = nullptr;
SLVolumeItf pcmPlayerVolume = nullptr;
//缓冲器队列接口
SLAndroidSimpleBufferQueueItf pcmBufferQueue;

FILE *pcmFile;
void *buffer;
uint8_t *out_buffer;
int count = 0;
int getPcmData(void **pcm) {
    LOGD("读取 = %d", count)
    int size = 0;
    while (!feof(pcmFile)) {
        size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        if (out_buffer == nullptr) {
            break;
        } else {
        }
        *pcm = out_buffer;
        count++;
        break;
    }
    return size;
}



void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf btf, void *context) {
    int size = getPcmData(&buffer);
    if (buffer != nullptr) {
        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, size);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidfmmpeg_NativeDemo_n_1testOpenSLES(JNIEnv *env, jobject thiz, jstring path) {
    const char *pcmurl = env->GetStringUTFChars(path, 0);
    errno = 0;
    pcmFile = fopen(pcmurl, "r");
    if (pcmFile == nullptr) {
        LOGD("pcmFile == nullptr %s %d", pcmurl, errno)
        return;
    }
    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);
    SLresult result;

    /**
     * 第一步
     *  创建引擎对象
     */
    slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    /**
     * 第二步
     * 创建混音器
     */
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    (void) result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (void) result;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverbItf);

    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverbItf)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverbItf, &reverbSettings);
    }
    result;

    /**
     * 第三步
    * 创建播放器
    */

    SLDataLocator_AndroidSimpleBufferQueue androidBufferQueue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM, //播放pcm格式的数据
                            2,//2个声道 （立体声）
                            SL_SAMPLINGRATE_44_1, // 44100hz 采样率
                            SL_PCMSAMPLEFORMAT_FIXED_16,//16位
                            SL_PCMSAMPLEFORMAT_FIXED_16,//和位数一致就行
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, //左声道|右声道
                            SL_BYTEORDER_LITTLEENDIAN //结束标记
    };
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSource slDataSource = {&androidBufferQueue, &pcm};
    SLDataSink audioSnk = {&outputMix, nullptr};
//    //设置参数
    const SLInterfaceID sids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource,
                                                &audioSnk, 3,
                                                sids, req);
    if (SL_RESULT_SUCCESS == result) {
        LOGD("初始化播放器成功")
    } else {
        LOGD("初始化播放器失败")
    }
    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);


    //得到接口后调用，获取Player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);
    /**
     * 第四步
     * 创建缓存区和回调函数
     */
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    //缓存接口回调
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack, nullptr);
    //获取音量接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_VOLUME, &pcmPlayerVolume);

    /**
     * 第五步
     * 设置播放状态
     */
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

    /**
     * 第六步
     * 主动回调函数开始工作
     */
    pcmBufferCallBack(pcmBufferQueue, nullptr);
    //销毁
    env->ReleaseStringUTFChars(path, pcmurl);
}