cmake_minimum_required(VERSION 3.2)
project(micromouse)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    maze/maze.cpp
    maze/maze.h)

add_executable(micromouse ${SOURCE_FILES} main/init_maze.h main/init_maze.cpp)