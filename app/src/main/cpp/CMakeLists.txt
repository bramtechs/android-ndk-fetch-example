cmake_minimum_required(VERSION 3.22.1)

project(fetchlib)

add_library(${PROJECT_NAME} SHARED fetch.cpp)
target_link_libraries(${PROJECT_NAME} android log)
