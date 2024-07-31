cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_SHARED_LINKER_FLAGS "-fPIC -shared -fvisibility=hidden -fvisibility-inlines-hidden")
project(Algorithm_208992883_322623182_randomized_DFS)
message(STATUS "PROJECT_SOURCE_DIR is: ${PROJECT_SOURCE_DIR}") 
set(INCLUDES    /Users/orshemesh/University/ThirdYear/CSTopics/cppHW01/algorithm
                ${PROJECT_SOURCE_DIR}/../common/)
set(SOURCES     ${PROJECT_SOURCE_DIR}/../algorithm/MyAlgorithm.cpp
                Algorithm_208992883_322623182_randomized_DFS.cpp)
message(SOURCES "INCLUDES is: ${SOURCES}")
add_library(${PROJECT_NAME} SHARED ${SOURCES})
message(STATUS "INCLUDES is: ${INCLUDES}")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
