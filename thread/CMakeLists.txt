cmake_minimum_required(VERSION 3.10)

project(thread)

add_library(threadlib STATIC worker_thread.cpp)

target_include_directories(threadlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
