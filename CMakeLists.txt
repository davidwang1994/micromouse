cmake_minimum_required(VERSION 3.3)
project(micromouse)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main/init_maze.cpp
    main/init_maze.h
    maze/maze.cpp
    maze/maze.h
    maze/maze_generator.cpp
    maze/maze_generator.h)

add_executable(micromouse ${SOURCE_FILES})