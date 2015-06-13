cmake_minimum_required(VERSION 3.2)
project(Module2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES main.c)
add_executable(Module2 ${SOURCE_FILES} )