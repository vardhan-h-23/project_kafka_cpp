# Root CMakeLists.txt for the project
# This file configures the overall build and includes subdirectories for modular components
cmake_minimum_required(VERSION 3.10)
project(KafkaCppProject)

set(cppkafka_DIR "/usr/local/lib/cmake/CppKafka")
add_subdirectory(consumer)
add_subdirectory(controller)
add_subdirectory(utils)
add_subdirectory(thread)
add_subdirectory(main)
add_subdirectory(order)
add_subdirectory(actor)
