cmake_minimum_required(VERSION 3.10)

project(order)

add_library(order STATIC order.cpp)

target_include_directories(order PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
