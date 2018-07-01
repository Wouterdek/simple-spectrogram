cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(simple_spectrogram VERSION 1.0 LANGUAGES CXX)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories("${PROJECT_SOURCE_DIR}/src")

set(APP_SRC_DIR "${PROJECT_SOURCE_DIR}/src/app")

add_executable(simple_spectrogram ${APP_SRC_DIR}/main.cpp ${APP_SRC_DIR}/lib/lodepng.cpp ${APP_SRC_DIR}/lib/AudioFile.cpp)
