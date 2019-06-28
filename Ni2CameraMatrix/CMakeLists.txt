### CMakeLists for Ni2CameraMatrix ###

cmake_minimum_required (VERSION 3.1)

set(TARGET_SAMPLE_NAME Ni2CameraMatrix)

project(${TARGET_SAMPLE_NAME})

## Setup Building Environment
include(${PROJECT_SOURCE_DIR}/../CMake/SetupBuildEnvironment.cmake)

## Build Ni2CameraMatrix Executable
add_executable(${TARGET_SAMPLE_NAME} ${PROJECT_SOURCE_DIR}/Source/Ni2CameraMatrix.cpp)

target_include_directories(${TARGET_SAMPLE_NAME} PRIVATE ${OPENNI2_INC})

target_link_libraries(${TARGET_SAMPLE_NAME} ${OPENNI2_LIB})

## Installation
install(TARGETS ${TARGET_SAMPLE_NAME} DESTINATION .)