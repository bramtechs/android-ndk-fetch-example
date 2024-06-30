#include <jni.h>
#include <string>

#define BRIDGE_METHOD(X) Java_com_doomhowl_fetchexample_FetchExampleActivity_##X

extern "C" JNIEXPORT jstring JNICALL BRIDGE_METHOD(stringFromJNI)(JNIEnv* env,
                                                 jobject /* this */) {
  std::string hello = "Hello world!";
  return env->NewStringUTF(hello.c_str());
}