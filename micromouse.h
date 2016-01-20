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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	unsigned char dist;
	bool top_wall;
	bool right_wall;

	Cell(int y, int x) : y(y), x(x), dist(0), top_wall(false), right_wall(false) {}
	Cell(int y, int x, int dist) : y(y), x(x), dist(dist), top_wall(false), right_wall(false) { if (y < 8 && x == 0) right_wall = true; }
	Cell(int y, int x, bool top_wall, bool right_wall) : y(y), x(x), top_wall(top_wall), right_wall(right_wall) {}
};

extern Cell *maze[MAZE_SIZE][MAZE_SIZE];

//int manhattan_dist(int x1, int x2, int y1, int y2)();
//
//int min4(int a, int b, int c, int d)();

//void update_distances(vector<Cell*> &stack)();

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
#ifndef __MAZE_GEN_H__
#define __MAZE_GEN_H__



//Generates a valid random maze with one solveable path and every cell is visitable 
void generateOnePath(Cell *maze[16][16]);

//Generates a valid random maze with multiple solveable paths and every cell is visitable
//By randomly tearing down wallsToKill number of walls. Does not tear down unremovable walls
void generateMoreThanOnePath(Cell *maze[16][16], int wallsToKill);

//Randomly tears down wallsToKill number of walls in existing maze. Does not tear down unremovable walls
//Must have enough walls left or will not terminate
void tearDownWalls(Cell *maze[16][16], int wallsToKill);

#endif