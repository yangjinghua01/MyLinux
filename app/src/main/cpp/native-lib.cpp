#include <jni.h>
#include <string>
// 日志输出
#include <android/log.h>

#define TAG "JNISTUDY"
// __VA_ARGS__ 代表 ...的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,  __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,  __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_mylinux_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT void JNICALL
/**
 * 纯静态注册
 * @param env
 * @param thiz
 */
Java_com_example_mylinux_MainActivity_staticRegister(JNIEnv *env, jobject thiz) {
    // TODO: implement staticRegister()
}
//TODO ==================》动态注册
JavaVM *javaVm = nullptr;//如果不用nullptr就是系统乱值C++11后取代初始化指针复制
const char *mainActivityClassName = "com/example/mylinux/MainActivity";
const char *javaMethod01 = "dynamicJavaMethod01";

//native真正的函数
void dynamicMethod01() {
    LOGE("我是动态注册的函数");
}

int dynamicMethod02() {
    LOGE("动态注册函数02");
}

static const JNINativeMethod jniNativeMethod[] = {
        {javaMethod01,          "()V",                   (void *) (dynamicMethod01)},
        {"dynamicJavaMethod02", "(Ljava/lang/String;)I", (int *) (dynamicMethod02)},
};
//默认有像java的构造函数如果你不写默认就有，如果你写了就有默认的构造函数
//JNI JNI_OnLoad函数如果你不写JNI_ONLoad默认就有JNI_OnLOad.如果你写JNI——OnLoad函数没写默认的JNI_OnLoad函数
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVm, void *) {
    ::javaVm = javaVm;
    JNIEnv *jniEnv = nullptr;
    int result = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    //result 等于0 就是成功 c库 ffmmpeg成功就是0
    if (result != JNI_OK) {
        return -1;
    }
    LOGE("System.LoadLibrary------------>""JNI Load init");
    jclass mainActivityClass = jniEnv->FindClass(mainActivityClassName);
    jniEnv->RegisterNatives(mainActivityClass, jniNativeMethod, sizeof(jniNativeMethod)/sizeof(JNINativeMethod));
    LOGE("注册没问题");
    return JNI_VERSION_1_6;//AS的jdk在JNI默认最高1.6
}