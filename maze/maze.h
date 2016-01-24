#ifndef __MAZE_H__
#define __MAZE_H__

#include <cmath>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define TOP 0
#define Right 1
#define Bottom 2
#define Left 3

using namespace std;

const int MAZE_SIZE = 16;

class Cell {

public:
    int y;
    int x;
    int dist;
//    unsigned char dist;
    bool top_wall;
    bool right_wall;

    Cell(int y, int x) : y(y), x(x), dist(0), top_wall(false), right_wall(false) {}
    Cell(int y, int x, int dist) : y(y), x(x), dist(dist), top_wall(false), right_wall(false) {if (y < 8 && x == 0) right_wall=true;}
    Cell(int y, int x, bool top_wall, bool right_wall) : y(y), x(x), top_wall(top_wall), right_wall(right_wall) {}
};

extern Cell *maze[MAZE_SIZE][MAZE_SIZE];
extern Cell *mazeIn[MAZE_SIZE][MAZE_SIZE];

//int manhattan_dist(int x1, int x2, int y1, int y2)();
//
//int min4(int a, int b, int c, int d)();

//void update_distances(vector<Cell*> &stack)();

bool fully_explored();

bool is_solved();

void clear();

void load();

void save();

void serialize(int rows, int cols);
void deserialize();
void print_debug_maze();
/***** Test functions *****/

void generate_random_walls();

void init_maze();

void print_maze();

#endif
