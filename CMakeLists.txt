# Root CMakeLists.txt for the project
# This file configures the overall build and includes subdirectories for modular components
cmake_minimum_required(VERSION 3.10)
project(KafkaCppProject)

set(CMAKE_BUILD_TYPE Debug) # Or RelWithDebInfo for partial optimization
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") # Full debug info, no optimization

set(cppkafka_DIR "/usr/local/lib/cmake/CppKafka")
add_subdirectory(consumer)
add_subdirectory(controller)
add_subdirectory(utils)
add_subdirectory(thread)
add_subdirectory(main)
add_subdirectory(order)
add_subdirectory(actor)
