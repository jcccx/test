#include <jni.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
//#include "optimizing_compiler.h"
//函数指针
typedef int (*CAC_FUNC)(int, int);
//void (*hit)(void*);
//void* (*hit)(bool*);
int (*func)(int,int);
#include <android/log.h>

#define TAG "SoTest" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

//动态链接库路径
#define LIB_CACULATE_PATH "libart-compiler.so"
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sotest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    void *handle;
    char *error;
    CAC_FUNC cac_func = NULL;

    //打开动态链接库
    handle = dlopen(LIB_CACULATE_PATH, RTLD_LAZY);
    if (handle) {
        LOGE("Loading lib successfully");
    }
    if (!handle) {
        LOGE("Loading lib failed:  %s", dlerror());
    }

    //清除之前存在的错误
    dlerror();
 /*
    hit = (void (*)(void*)) dlsym(handle, "jit_unload");
    if (hit)  {
        LOGE("Loading libflush error");
    }

    hit = (void* (*)(bool*)) dlsym(handle, "jit_load");
    if (!hit)  {
        LOGE("Loading libflush error");
    }
    bool *a =NULL;
    hit(a);
*/
    //获取一个函数
    func = (int (*)(int,int)) dlsym(handle, "myAdd");
    if (!func)  {
        LOGE("Loading func error");
    }
    if ((error = dlerror()) != NULL)  {
        LOGE("Load func failed:  %s",error);
    }
    LOGE("myAdd: %d\n", func(2,7));

    func = (int (*)(int,int)) dlsym(handle, "myMax");
    if ((error = dlerror()) != NULL)  {
        LOGE("Load func failed:  %s",error);
    }
    LOGE("myMax: %d\n", func(2,7));

/*    *(void **) (&cac_func) = dlsym(handle, "add");
    if ((error = dlerror()) != NULL)  {
        LOGE("Load lib failed:  %s",error);
    }
    LOGD("myAdd: %d\n", (*cac_func)(2,7));

    cac_func = (CAC_FUNC)dlsym(handle, "myMax");
    LOGD("myMax: %d\n", cac_func(2,7));

    cac_func = (CAC_FUNC)dlsym(handle, "mySwap");
    printf("mySwap: a: %d,b: %d\n",a,b );
*/

    //关闭动态链接库
    dlclose(handle);
    //exit(EXIT_SUCCESS);
    return env->NewStringUTF("");
}