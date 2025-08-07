cmake_minimum_required(VERSION 3.10)

project(actor)

add_library(actor STATIC actor_master.cpp)

target_include_directories(actor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
