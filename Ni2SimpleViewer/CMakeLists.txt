### CMakeLists for Ni2SimpleViewer ###

cmake_minimum_required (VERSION 3.1)

set(TARGET_SAMPLE_NAME Ni2SimpleViewer)

project(${TARGET_SAMPLE_NAME})

## Setup Building Environment
include(${PROJECT_SOURCE_DIR}/../CMake/SetupBuildEnvironment.cmake)
include(${PROJECT_SOURCE_DIR}/../CMake/SetupOpenCV.cmake)

## Definitions
if(WINDOWS)
    add_definitions(/DSCREEN_CLEAR_CMD="cls")
elseif(LINUX)
    add_definitions(-DSCREEN_CLEAR_CMD="clear")
else()
endif()

## Build Ni2SimpleViewer Executable
add_executable(${TARGET_SAMPLE_NAME} ${PROJECT_SOURCE_DIR}/Source/Ni2SimpleViewer.cpp)

target_include_directories(${TARGET_SAMPLE_NAME} PRIVATE ${OPENNI2_INC})
target_include_directories(${TARGET_SAMPLE_NAME} PRIVATE ${OPENCV_INC})

target_link_libraries(${TARGET_SAMPLE_NAME} ${OPENNI2_LIB})
target_link_libraries(${TARGET_SAMPLE_NAME} ${OPENCV_LIB})

## Installation
install(TARGETS ${TARGET_SAMPLE_NAME} DESTINATION .)