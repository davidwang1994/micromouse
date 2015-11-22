#ifndef __MAZE_H __
#define __MAZE_H __

#include <cmath.h>
#include <iostream>
#include <string>

const int MAZE_SIZE = 16;

extern unsigned char maze[MAZE_SIZE][MAZE_SIZE]; 

void init_maze();

void print_maze();

#endif
