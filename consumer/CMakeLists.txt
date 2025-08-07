cmake_minimum_required(VERSION 3.10)

project(consumer)

# Find cppkafka
find_package(CppKafka REQUIRED)
# Find Boost (for cppkafka)
find_package(Boost REQUIRED)
# Find nlohmann_json (header-only, optional)
find_package(nlohmann_json QUIET)

add_library(consumer STATIC consumer.cpp)

# Link libraries
if(nlohmann_json_FOUND)
    target_link_libraries(consumer PRIVATE CppKafka::cppkafka Boost::boost nlohmann_json::nlohmann_json)
else()
    target_link_libraries(consumer PRIVATE CppKafka::cppkafka Boost::boost)
endif()

# Include directories (if needed)
target_include_directories(consumer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
