#ifndef __MAZE_H __
#define __MAZE_H __

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#define TOP 0
#define Right 1
#define Bottom 2
#define Left 3

const int MAZE_SIZE = 16;

extern Cell *maze[MAZE_SIZE][MAZE_SIZE];

class Cell {

public:
    int x;
    int y;
    unsigned char dist;
    bool top_wall;
    bool right_wall;

    Cell() : x(x), y(y), dist(0), top_wall(false), right_wall(false) {}
    Cell(int dist) : x(x), y(y), dist(dist), top_wall(false), right_wall(false) { }

};

void init_maze();

void print_maze();

#endif
