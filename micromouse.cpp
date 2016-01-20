// micromouse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "micromouse.h"



//Uses Kruskal's to generate a maze
//Borrowed from http://www.geeksforgeeks.org/greedy-algorithms-set-2-kruskals-minimum-spanning-tree-mst/


// a structure to represent a weighted edge in graph
struct Edge
{
	int src, dest, weight;
};

// a structure to represent a connected, undirected and weighted graph
struct Graph
{
	// V-> Number of vertices, E-> Number of edges
	int V, E;

	// graph is represented as an array of edges. Since the graph is
	// undirected, the edge from src to dest is also edge from dest
	// to src. Both are counted as 1 edge here.
	struct Edge* edge;
};

// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;
	graph->E = E;

	graph->edge = (struct Edge*) malloc(graph->E * sizeof(struct Edge));

	return graph;
}

// A structure to represent a subset for union-find
struct subset
{
	int parent;
	int rank;
};

// A utility function to find set of an element i
// (uses path compression technique)
int find(struct subset subsets[], int i)
{
	// find root and make root as parent of i (path compression)
	if (subsets[i].parent != i)
		subsets[i].parent = find(subsets, subsets[i].parent);

	return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(struct subset subsets[], int x, int y)
{
	int xroot = find(subsets, x);
	int yroot = find(subsets, y);

	// Attach smaller rank tree under root of high rank tree
	// (Union by Rank)
	if (subsets[xroot].rank < subsets[yroot].rank)
		subsets[xroot].parent = yroot;
	else if (subsets[xroot].rank > subsets[yroot].rank)
		subsets[yroot].parent = xroot;

	// If ranks are same, then make one as root and increment
	// its rank by one
	else
	{
		subsets[yroot].parent = xroot;
		subsets[xroot].rank++;
	}
}

// Compare two edges according to their weights.
// Used in qsort() for sorting an array of edges
int myComp(const void* a, const void* b)
{
	struct Edge* a1 = (struct Edge*)a;
	struct Edge* b1 = (struct Edge*)b;
	return a1->weight > b1->weight;
}

// The main function to construct MST using Kruskal's algorithm
void KruskalMST(struct Graph* graph, Cell *maze[16][16])
{
	int V = 480;
	struct Edge result[480];  // Tnis will store the resultant MST
	int e = 0;  // An index variable, used for result[]
	int i = 0;  // An index variable, used for sorted edges

				// Step 1:  Sort all the edges in non-decreasing order of their weight
				// If we are not allowed to change the given graph, we can create a copy of
				// array of edges
	qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);

	// Allocate memory for creating V ssubsets
	struct subset *subsets =
		(struct subset*) malloc(V * sizeof(struct subset));

	// Create V subsets with single elements
	for (int v = 0; v < V; ++v)
	{
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}

	// Number of edges to be taken is equal to V-1
	while (e < V - 1)
	{
		// Step 2: Pick the smallest edge. And increment the index
		// for next iteration
		struct Edge next_edge = graph->edge[i++];

		int x = find(subsets, next_edge.src);
		int y = find(subsets, next_edge.dest);

		// If including this edge does't cause cycle, include it
		// in result and increment the index of result for next edge
		if (x != y)
		{
			result[e++] = next_edge;
			Union(subsets, x, y);
		}
		// Else discard the next_edge
	}

	// print the contents of result[] to display the built MST
	printf("Following are the edges in the constructed MST\n");
	for (i = 0; i < e; ++i)
		printf("%d -- %d == %d\n", result[i].src, result[i].dest,
			result[i].weight);




	// Opens up the corresponding walls in maze
	int src, row, col;
	for (int i = 0; i < e; i++) {
		src = result[i].src;
		row = 15 - (src / 16);
		col = src % 16;
		if (result[i].dest - src == 1) { //Horizontal cells
			maze[row][col]->right_wall = false;
		}
		else { //Vertical cells
			maze[row][col]->top_wall = false;
		}
	}

}


/*
* Generates the 16 by 16 maze, storing into given 16*16 array of Cells
* Only one path is actually possible
*/
void generateOnePath(Cell *maze[16][16])
{

	int V = 256;  // Number of vertices in graph
	int E = 480;  // Number of edges in graph
	struct Graph* graph = createGraph(V, E);

	//Vertices start from bottom left to top right, 0-255, in this generator file only.
	Edge* edge = graph->edge;

	//Random weights are 100 - 355, with sentinels 0 (must have) and 99999 (must not have)

	int thisVert;
	for (int i = 0; i < 16; i++) { //For each row
		for (int j = 0; j < 15; j++) { //Generate through that row
			thisVert = i * 16 + j;
			edge[thisVert].src = thisVert;
			edge[thisVert].dest = thisVert + 1;
			edge[thisVert].weight = rand() % 256 + 100;
		}
	}
	for (int i = 0; i < 16; i++) { //For each column
		for (int j = 0; j < 15; j++) { //Generate through that column
			thisVert = i + j * 16 + 240;
			edge[thisVert].src = thisVert;
			edge[thisVert].dest = thisVert + 16;
			edge[thisVert].weight = rand() % 256 + 100;
		}
	}

	//Sets sentinel values for certain edges. 0, center: 119, 120, 135, 136

	//0->1 is wall, 0->16 is not wall
	edge[0].weight = 99999;
	edge[240].weight = 0;
	//Horizontal middle
	edge[127].weight = 99999; edge[128].weight = 0; edge[129].weight = 99999;
	edge[112].weight = 99999; edge[113].weight = 0; edge[114].weight = 99999;
	//Vertical middle 
	edge[367].weight = 99999; edge[368].weight = 0; edge[369].weight = 99999;
	edge[352].weight = 99999; edge[353].weight = 0; edge[354].weight = 99999;
	//Randomly open one of middle walls
	int middleWalls[8] = { 127, 129, 112, 114, 367, 369, 352, 354 };
	edge[middleWalls[rand() % 7]].weight = 0;


	//Initializes all walls in maze
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			maze[i][j] = new Cell(i, j, true, true);
		}
	}

	//Find the walls to open and opens them
	KruskalMST(graph, maze);
}

void generateMoreThanOnePath(Cell *maze[16][16], int wallsToKill) {
	generateOnePath(maze);
	tearDownWalls(maze, wallsToKill);
}

void tearDownWalls(Cell *maze[16][16], int wallsToKill) {
	int num;
	Cell * c;
	while (wallsToKill > 0) {
		num = rand() % 480;
		if (num == 0 || num == 240 || num == 127 || num == 129 || num == 112 || num == 114 || num == 367 || num == 369 || num == 352 || num == 354) {
			continue;//Middle walls or walls of bottom left cell
		}
		if (num > 240) { //Top walls
			num -= 240;
			c = maze[15 - num / 16][num % 16];
			if (c->top_wall) {
				c->top_wall = true;
				wallsToKill--;
			}
		}
		else { //right walls
			c = maze[15 - num / 15][num % 15];
			if (c->right_wall) {
				c->right_wall = false;
				wallsToKill--;
			}
		}
	}
}

Cell *maze[MAZE_SIZE][MAZE_SIZE];

/*
* Calculates the total number of cells needed to get from a point (x1, y1)
* to a point (x2, y2).
*/
int manhattan_dist(int x1, int x2, int y1, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

/*
* Function that takes the minimum of the four given distances
*/
int min4(int a, int b, int c, int d) {
	int min;
	(a < b) ? min = a : min = b;
	if (c < min) min = c;
	if (d < min) min = d;
	return min;
}

int min_open_neighbor(vector<Cell*> cells) {
	int min = INT_MAX;
	for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); it++)
		if ((*it)->dist < min) {
			min = (*it)->dist;
		}
	return min;
}

bool is_center(Cell *cell) {
	int x = cell->x;
	int y = cell->y;
	int goal1 = MAZE_SIZE / 2;
	int goal2 = (MAZE_SIZE - 1) / 2;
	if (manhattan_dist(y, goal1, x, goal1) == 0 ||
		manhattan_dist(y, goal1, x, goal2) == 0 ||
		manhattan_dist(y, goal2, x, goal1) == 0 ||
		manhattan_dist(y, goal2, x, goal2) == 0) {
		return true;
	}
	return false;
}

/*
* Initializes the maze using the manhattan distances as the starting distances.
*/
void init_maze() {
	int goal1 = MAZE_SIZE / 2;
	int goal2 = (MAZE_SIZE - 1) / 2;
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			// Distance of the cell will be the minimum distance to the closest
			// one out of four middle destination cells.
			maze[i][j] = new Cell(i, j, min4(manhattan_dist(i, goal1, j, goal1),
				manhattan_dist(i, goal1, j, goal2),
				manhattan_dist(i, goal2, j, goal1),
				manhattan_dist(i, goal2, j, goal2)));
		}
	}
}

/*
* Initializes Cell * maze from given maze text file (formatted similar to http://www.tcp4me.com/mmr/mazes/)
* Assumes 33*33 text grid in file and only checks first 32 rows and for - | ' ' at their correct positions
* A new cell constructor was added for just posiiton and walls.
*/
void load_maze(string file_name) {
	ifstream file(file_name);
	string top, right;
	int row = 0;
	while (row < 16) {
		getline(file, top);
		getline(file, right);
		for (int col = 0; col < 16; col++) {
			maze[row][col] = new Cell(row, col, top[(col * 2) + 1] == '-', right[(col * 2) + 2] == '|');
			cout << (top[(col * 2) + 1] == '-') << (right[(col * 2) + 2] == '|') << "\n";
		}
		cout << "\n";
		row++;
	}
}

/*
* Initializes the default (blank maze) manhattan distance values into existing maze
*/
void init_dist() {
	int goal1 = MAZE_SIZE / 2;
	int goal2 = (MAZE_SIZE - 1) / 2;
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			// Distance of the cell will be the minimum distance to the closest
			// one out of four middle destination cells.
			maze[i][j]->dist = min4(manhattan_dist(i, goal1, j, goal1),
				manhattan_dist(i, goal1, j, goal2),
				manhattan_dist(i, goal2, j, goal1),
				manhattan_dist(i, goal2, j, goal2));
		}
	}
}

void add_cell_to_update(vector<Cell*> &stack, Cell *cell) {
	stack.push_back(cell);
}

/*
* Function to update the distances of the cells
*/
void update_distances(vector<Cell*> &stack) {
	Cell *current;
	vector<Cell *> open_neighbors;
	vector<Cell*> neighbors;

	int neighbor_dist[4];
	int x, y;
	int min;
	while (!stack.empty()) {
		current = stack.back();
		cout << "stack not empty" << endl;
		cout << "current cell: (" << current->x << "," << current->y << ")" << endl;
		stack.pop_back();
		x = current->x;
		y = current->y;
		// check top neighbor
		if (y < MAZE_SIZE - 1) {
			neighbors.push_back(maze[y + 1][x]);
			if (!current->top_wall)
				open_neighbors.push_back(maze[y + 1][x]);
		}
		// check right neighbor
		if (x < MAZE_SIZE - 1) {
			neighbors.push_back(maze[y][x + 1]);
			if (!current->right_wall)
				open_neighbors.push_back(maze[y][x + 1]);
		}
		// check bottom neighbor
		if (y > 0) {
			neighbors.push_back(maze[y - 1][x]);
			if (maze[y - 1][x]->top_wall) {
				open_neighbors.push_back(maze[y - 1][x]);
			}
		}
		// check left neighbor
		if (x > 0) {
			neighbors.push_back(maze[y][x - 1]);
			if (maze[y][x - 1]->right_wall) {
				open_neighbors.push_back(maze[y][x - 1]);
			}
		}
		min = min_open_neighbor(open_neighbors);
		open_neighbors.clear();
		if (current->dist - 1 != min) {
			cout << "min: " << min << endl;
			current->dist = min + 1;
			for (vector<Cell *>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
				if (!is_center(*it)) {
					stack.push_back(*it);
					cout << "pushed cell: (" << (*it)->x << "," << (*it)->y << ")" << endl;
				}
			}
			neighbors.clear();
		}
	}
}

/*
* Generates random walls for the maze. Call this function
* before you want to print out the maze. Mostly used for
* testing the print_maze function.
*
* This function will print out above the maze whether or not there
* exists a wall for the top or right. The first space stands for whether
* there is a top wall, the second space stands for wether there is a right
* wall. (TR, T_, _R, __) The following 4 possibilities stand for a cell having
* (top and right, only top, only right, or no walls at all).
*
*/
void generate_random_walls() {
	string s;
	srand((unsigned)time(0));
	cout << time(0) << endl;
	int numCells = MAZE_SIZE * MAZE_SIZE;
	int numWalls = rand() % (numCells / 4) + (numCells * 3 / 4);
	cout << rand() << endl;
	cout << numWalls << endl;

	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			int y = MAZE_SIZE - 1 - i;
			int type = rand() % 4;
			if (numWalls > 0) {
				if (type == 1) {
					maze[y][j]->top_wall = true;
					numWalls--;
					s += "T_\t";
				}
				else if (type == 2) {
					maze[y][j]->right_wall = true;
					numWalls--;
					s += "_R\t";
				}
				else if (type == 3) {
					maze[y][j]->top_wall = true;
					maze[y][j]->right_wall = true;
					numWalls--;
					s += "TR\t";
				}
				else {
					s += "__\t";
				}
			}
			else {
				s += "__\t";
			}
		}
		s += "\n";
	}
	cout << s << endl;
}

/*
* How the maze should look like with a
* 4 x 4 maze example
*
*                      j
*
*          0     6     12    18    24
*
*           (3,0) (3,1)
*     0     _______________________
|     |     |           |
*          | 256 | 256 | 256   256 |
*     3    |     |_____|_____ _____|
|                 |     |
*          | 256   256   256 | 256 | (2,3)
*  i  6    |_____ _____      |_____|
|     |     |           |
*          | 256 | 256 | 256   256 |
*     9    |     |_____|      _____|
|                 |     |
*          | 256   256   256 | 256 |
*     12   |_________________|_____|
*
*           (0,0) (0,1)
*
*           y = (MAX_SIZE - 1 - (i / 3)
*           x = j / 6
*
*
* Ended up with too many bugs trying to implement the top
* maze, so went with this design instead.
*
*           +---+---+---+---+
*           |256 256|256 256|
*           +---+   +   +   +
*           |256 256|256+256|
*           +   +   +   +---+
*           |256 256|256 256|
*           +---+   +   +   +
*           |256 256 256 256|
*           +---+---+---+---+
*
*/

/*
* Function to print out an ascii representation of the maze.
*/
void print_maze() {

	// print top wall
	for (int i = 0; i < MAZE_SIZE; i++) {
		cout << "+---";
	}
	cout << "+\n";

	int rows = MAZE_SIZE + (MAZE_SIZE - 1);
	int y;

	for (int i = 0; i < rows; i++) {
		if (i % 2 == 0) {
			y = MAZE_SIZE - 1 - i / 2;
		}
		else {
			y = MAZE_SIZE - 1 - (i / 2 + 1);
		}
		for (int j = 0; j < MAZE_SIZE; j++) {

			if (i % 2 != 0) {
				if (maze[y][j]->top_wall) {
					cout << "+---";
				}
				else {
					cout << "+   ";
				}
				if (j == MAZE_SIZE - 1) {
					cout << "+";
				}
			}

			else {
				if (j == 0) {
					cout << "|";
				}

				int dist = maze[y][j]->dist;
				cout << "   ";
				/*if (dist > 99) {
					cout << dist;
				}
				else if (dist > 9) {
					cout << " " << dist;
				}
				else {
					cout << " " << dist << " ";
				}*/

				if (maze[y][j]->right_wall || j == MAZE_SIZE - 1) {
					cout << "|";
				}
				else {
					cout << " ";
				}
			}
		}
		cout << "\n";
	}

	// print bottom wall
	for (int i = 0; i < MAZE_SIZE; i++) {
		cout << "+---";
	}
	cout << "+\n";
}


int main() {
	//    init_maze();
	////    generate_random_walls();
	//    print_maze();
	//    vector<Cell*> cells;
	//    cells.push_back(maze[0][0]);
	//    add_cell_to_update(cells, maze[7][0]);
	//    update_distances(cells);
	
	load_maze("2008japan.maze");
	//generateOnePath(maze);
	print_maze();
	
	getchar();
}

/*
void floodfill(int[][] maze, int x, int y) {
if (maze[x][y] != visited) {
maze[x][y] = true;
if (x < 8 && y < 8) {
floodfill(maze, x+1, y+1, true)
}
}
}
*/


