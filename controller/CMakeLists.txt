cmake_minimum_required(VERSION 3.10)

project(controller)

add_library(controller STATIC controller.cpp)

target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(controller PUBLIC threadlib actor order)
