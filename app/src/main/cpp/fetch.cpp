#include <curl/curl.h>
#include <android/log.h>
#include <jni.h>
#include <iostream>
#include <sstream>
#include <string>

#define BRIDGE_METHOD(X) Java_com_doomhowl_fetchexample_FetchExampleActivity_##X

extern "C" void startLogger(const char* name);

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::vector<char>& data) {
  data.insert(data.end(), (char*)ptr, (char*)ptr + size * nmemb);
  return size * nmemb;
}

extern "C" JNIEXPORT jbyteArray BRIDGE_METHOD(downloadImage)(JNIEnv* env, jobject obj){
  startLogger("fetchexample");

  // enable curl with openssl
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://doomhowl-interactive.com/files/android-ndk-fetch-example/wolf.jpg");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    std::vector<char> data{};
    std::string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    // Error handling
    if (res != CURLE_OK) {
      std::cerr << "curl request failed! :: " << curl_easy_strerror(res) << '\n';
      curl_easy_cleanup(curl);
        return nullptr;
    }

    char* url = nullptr;
    long response_code = 0;
    double elapsed = 0.0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);

    curl_easy_cleanup(curl);

    std::cout << "Code: " << response_code << '\n';

    // Get the size of the vector
    jsize size = static_cast<jsize>(data.size());

    // Create a new byte array in Java
    jbyteArray result = env->NewByteArray(size);
    if (result == nullptr) {
      return nullptr; // Out of memory error thrown
    }

    // Copy the vector data to the byte array
    env->SetByteArrayRegion(result, 0, size, reinterpret_cast<const jbyte*>(data.data()));

    return result;
  }
  return nullptr;
}
