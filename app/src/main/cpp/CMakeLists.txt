cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
project(fetchlib)

message("Processor used ${CMAKE_SYSTEM_PROCESSOR}")
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(CURL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7-a")
    set(CURL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")
    set(CURL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/x86_64)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86" OR CMAKE_SYSTEM_PROCESSOR MATCHES "i686")
    set(CURL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/x86)
else()
    message(FATAL_ERROR "Unknown architecture ${CMAKE_SYSTEM_PROCESSOR}!")
endif()

add_library(${PROJECT_NAME} SHARED fetch.cpp log_forwarder.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/include)

target_link_libraries(${PROJECT_NAME}
        android
        log
        ${CURL_LIB_PATH}/libcurl.a
        ${CURL_LIB_PATH}/libcrypto.a
        ${CURL_LIB_PATH}/libssl.a
        -lz
)
