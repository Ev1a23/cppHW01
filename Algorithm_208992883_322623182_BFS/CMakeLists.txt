cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(Algorithm_208992883_322623182_BFS)

set(INCLUDES    /Users/orshemesh/University/ThirdYear/CSTopics/cppHW01/algorithm
                ${PROJECT_SOURCE_DIR}/../common/)

set(SOURCES     ${PROJECT_SOURCE_DIR}/../algorithm/MyAlgorithm.cpp
                Algorithm_208992883_322623182_BFS.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

set(ALGO_LIB_DIR ${CMAKE_BINARY_DIR}/algo_dir)

file(MAKE_DIRECTORY ${ALGO_LIB_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${ALGO_LIB_DIR}
)