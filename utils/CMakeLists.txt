cmake_minimum_required(VERSION 3.10)

project(utils)

add_library(utils INTERFACE)

target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
