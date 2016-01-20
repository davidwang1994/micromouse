#ifndef __MAZE_GEN_H__
#define __MAZE_GEN_H__

int middleWalls[8] = {127, 129, 112, 114, 367, 369, 352, 354}; //unordered middle walls (out of 480), valid regardless of direction

//Generates a valid random maze with one solveable path and every cell is visitable 
void generateOnePath(Cell ***maze);


//Generates a valid random maze with multiple solveable paths and every cell is visitable
//By randomly tearing down wallsToKill number of walls. Does not tear down unremovable walls
void generateMoreThanOnePath(Cell ***maze, int wallsToKill);

//Randomly tears down wallsToKill number of walls in existing maze. Does not tear down unremovable walls
//Must have enough walls left or will not terminate
void tearDownWalls(Cell ***maze, int wallsToKill);

#endif