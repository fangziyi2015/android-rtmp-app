#include <jni.h>
#include <string>
#include <rtmp.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_rtmp_androidapp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}