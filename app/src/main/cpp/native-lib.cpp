#include <jni.h>
#include <string>
#include <rtmp.h>
#include <x264.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_rtmp_androidapp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    jint version = RTMP_LibVersion();
    char buffer[40];
    sprintf(buffer, "rtmp的版本是：%d", version);
    x264_picture_t *p = new x264_picture_t;
    return env->NewStringUTF(buffer);
}