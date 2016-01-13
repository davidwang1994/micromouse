#ifndef __MAZE_H__
#define __MAZE_H__

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define TOP 0
#define Right 1
#define Bottom 2
#define Left 3

using namespace std;

const int MAZE_SIZE = 2;

class Cell {

public:
    int y;
    int x;
    unsigned char dist;
    bool top_wall;
    bool right_wall;

    Cell(int y, int x) : y(y), x(x), dist(0), top_wall(false), right_wall(false) {}
    Cell(int y, int x, int dist) : y(y), x(x), dist(dist), top_wall(false), right_wall(false) {if (y == 1 && x == 1) top_wall= true;}

};

extern Cell *maze[MAZE_SIZE][MAZE_SIZE];

int manhattan_dist(int x1, int x2, int y1, int y2)();

int min4(int a, int b, int c, int d)();

void update_distances(vector<Cell*> &stack)();

bool fully_explored();

bool is_solved();

void clear();

void load();

void save();



/***** Test functions *****/

void generate_random_walls();

void init_maze();

void print_maze();

#endif
