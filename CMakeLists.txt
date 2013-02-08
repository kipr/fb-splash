project(fb-splash)

cmake_minimum_required(VERSION 2.8.0)

add_executable(fb-splash src/main.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/deploy)