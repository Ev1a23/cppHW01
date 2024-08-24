cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
SET(CMAKE_EXE_LINKER_FLAGS  "-rdynamic")

project(my_robot)
message(${PROJECT_SOURCE_DIR})

set(SIM_DIR     ${PROJECT_SOURCE_DIR})
set(COMMON_DIR  ${PROJECT_SOURCE_DIR}/../common)
add_executable  (${PROJECT_NAME}  
                                ${SIM_DIR}/Main.cpp    
                                ${SIM_DIR}/MySimulator.cpp 
                                ${SIM_DIR}/AlgorithmRegistrar.cpp
                                ${SIM_DIR}/House.cpp
                                ${SIM_DIR}/my_battery_meter.cpp
                                ${SIM_DIR}/my_wall_sensor.cpp
                                ${SIM_DIR}/my_dirt_sensor.cpp
                )


target_include_directories  (${PROJECT_NAME} PRIVATE
                                            ${SIM_DIR}
                                            ${COMMON_DIR}
                                            ${PROJECT_SOURCE_DIR}/..

                            )

target_link_libraries   (${PROJECT_NAME} PRIVATE
                                        dl
                                        pthread
                        )