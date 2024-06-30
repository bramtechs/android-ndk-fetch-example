#include <curl/curl.h>
#include <android/log.h>
#include <jni.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BRIDGE_METHOD(X) Java_com_doomhowl_fetchexample_FetchExampleActivity_##X

static int pfd[2];
static pthread_t thr;
static const char *tag = "myapp";

static void *thread_func(void*)
{
  ssize_t rdsz;
  char buf[128];
  while((rdsz = read(pfd[0], buf, sizeof buf - 1)) > 0) {
    if(buf[rdsz - 1] == '\n') --rdsz;
    buf[rdsz] = 0;  /* add null-terminator */
    __android_log_write(ANDROID_LOG_DEBUG, tag, buf);
  }
  return 0;
}

int start_logger(const char *app_name)
{
  tag = app_name;

  /* make stdout line-buffered and stderr unbuffered */
  setvbuf(stdout, 0, _IOLBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  /* create the pipe and redirect stdout and stderr */
  pipe(pfd);
  dup2(pfd[1], 1);
  dup2(pfd[1], 2);

  /* spawn the logging thread */
  if(pthread_create(&thr, 0, thread_func, 0) == -1)
    return -1;
  pthread_detach(thr);
  return 0;
}

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*)ptr, size * nmemb);
  return size * nmemb;
}

extern "C" JNIEXPORT jstring JNICALL
BRIDGE_METHOD(stringFromJNI)(JNIEnv* env, jobject /* this */) {
  start_logger("fetchexample");
  start_logger("fetchexample");

  CURL* curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.be");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    std::string response_string;
    std::string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    // Error handling
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
      curl_easy_cleanup(curl);
      return env->NewStringUTF("Curl request failed");
    }

    char* url = nullptr;
    long response_code = 0;
    double elapsed = 0.0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);

    curl_easy_cleanup(curl);

    std::cout << "Code: " << response_code << '\n';
    std::cout << "Message: " << response_string << '\n';

    std::string trim = "";
    for (auto i = 0; i < 100; i++) {
      trim += response_string[i];
    }
    return env->NewStringUTF(trim.c_str());
  }

  return env->NewStringUTF("no curl");
}