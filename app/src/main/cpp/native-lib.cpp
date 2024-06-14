#include <jni.h>
#include <string>
#include <rtmp.h>
#include <x264.h>

#include "log4c.h"
#include "safe_queue.h"
#include "VideoChannel.h"
#include "util.h"

pthread_t pid_rtmp;
SafeQueue<RTMPPacket *> packets;
bool isReadyPushing;
bool isStart = false;

::uint32_t start_time;

VideoChannel *videoChannel;

void releaseRTMPPacket(RTMPPacket **packet) {
    if (packet) {
        RTMPPacket_Free(*packet);
        delete packet;
        packet = nullptr;
    }
}

void pushCallback(RTMPPacket *packet) {
    if (packet) {
        if (packet->m_nTimeStamp == -1) {
            packet->m_nTimeStamp = RTMP_GetTime() - start_time;
        }
        packets.push(packet);
    }
}

void *start_rtmp(void *ptr) {
    char *url = static_cast< char *>(ptr);
    int ret = 0;
    RTMP *rtmp = nullptr;
    do {
        // 1、申请内存
        rtmp = RTMP_Alloc();
        RTMP_Init(rtmp);
        rtmp->Link.timeout = 5;

        // 2、设置推流地址
        ret = RTMP_SetupURL(rtmp, url);
        if (!ret) {
            LOGE("设置url失败")
            break;
        }

        // 3、开启输出模式
        RTMP_EnableWrite(rtmp);
        // 连接服务器
        ret = RTMP_Connect(rtmp, nullptr);
        if (!ret) {
            LOGE("连接失败")
            break;
        }

        // 4、连接到流
        ret = RTMP_ConnectStream(rtmp, 0);
        if (!ret) {
            LOGE("连接到流失败")
            break;
        }
        start_time = RTMP_GetTime(); // 后面你就明白
        isReadyPushing = true;
        packets.setWork(true);
        RTMPPacket *packet = nullptr;
        while (isReadyPushing) {
            ret = packets.pop(packet);

            if (!ret) {
                continue;
            }
            if (!isReadyPushing) {
                break;
            }

            packet->m_nInfoField2 = rtmp->m_stream_id;

            // 5、发送数据
            ret = RTMP_SendPacket(rtmp, packet, 1);
            if (!ret) {
                LOGE("发送失败")
                break;
            }

            releaseRTMPPacket(&packet);
        }
        releaseRTMPPacket(&packet);
    } while (false);

    isStart = false;
    isReadyPushing = false;
    packets.setWork(false);
    packets.clear();
    if (rtmp) {
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
    }

    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rtmp_androidapp_PushHelper_initNative(JNIEnv *env, jobject thiz) {
    videoChannel = new VideoChannel();
    videoChannel->setPushCallback(pushCallback);

    // 设置释放回调
    packets.setReleaseCallback(releaseRTMPPacket);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rtmp_androidapp_PushHelper_startLiveNative(JNIEnv *env, jobject thiz, jstring url) {
    if (isStart) {
        return;
    }
    isStart = true;
    const char *c_path = env->GetStringUTFChars(url, nullptr);
    char *c_url = new char(strlen(c_path) + 1);
    strcpy(c_url, c_url);
    pthread_create(
            &pid_rtmp,
            nullptr,
            start_rtmp,
            c_url
    );
    env->ReleaseStringUTFChars(url, c_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rtmp_androidapp_PushHelper_initVideoEncoderNative(JNIEnv *env, jobject thiz, jint width,
                                                           jint height, jint fps, jint bitrate) {
    if (videoChannel) {
        videoChannel->initEncoder(width, height, fps, bitrate);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_rtmp_androidapp_PushHelper_startPushVideoNative(JNIEnv *env, jobject thiz,
                                                         jbyteArray _data) {
    // 健壮性判定
    if (!videoChannel || !isReadyPushing) {
        return;
    }

    jbyte *data = env->GetByteArrayElements(_data, nullptr);
    if (videoChannel) {
        videoChannel->encodeData(data);
    }
    env->ReleaseByteArrayElements(_data, data, 0);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_rtmp_androidapp_PushHelper_stopLiveNative(JNIEnv *env, jobject thiz) {
    isStart = false;
    isReadyPushing = false;
    packets.setWork(false);
    packets.clear();
    pthread_join(pid_rtmp, nullptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rtmp_androidapp_PushHelper_releaseNative(JNIEnv *env, jobject thiz) {
    // 释放资源
    DELETE(videoChannel)

}