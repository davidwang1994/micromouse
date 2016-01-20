//Uses Kruskal's to generate a maze
//Borrowed from http://www.geeksforgeeks.org/greedy-algorithms-set-2-kruskals-minimum-spanning-tree-mst/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maze.h"
#include "maze_generator.h"

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
    struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
    graph->V = V;
    graph->E = E;
 
    graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
 
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
void KruskalMST(struct Graph* graph, Cell ***maze)
{
    int V = graph->V;
    struct Edge result[V];  // Tnis will store the resultant MST
    int e = 0;  // An index variable, used for result[]
    int i = 0;  // An index variable, used for sorted edges
 
    // Step 1:  Sort all the edges in non-decreasing order of their weight
    // If we are not allowed to change the given graph, we can create a copy of
    // array of edges
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp);
 
    // Allocate memory for creating V ssubsets
    struct subset *subsets =
        (struct subset*) malloc( V * sizeof(struct subset) );
 
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
	for (int i = 0; i < e; i++){
		src = result[i].src;
		row = 15 - (src / 16);
		col = src % 16;
		if (result[i].dest - src == 1){ //Horizontal cells
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
void generateOnePath(Cell ***maze)
{

    int V = 256;  // Number of vertices in graph
    int E = 480;  // Number of edges in graph
    struct Graph* graph = createGraph(V, E);
 	
	//Vertices start from bottom left to top right, 0-255, in this generator file only.
	Edge* edge = graph->edge;
	
	//Random weights are 100 - 355, with sentinels 0 (must have) and 99999 (must not have)
	
	int thisVert;
    for (int i = 0; i < 16; i++){ //For each row
		for (int j = 0; j < 15; j++){ //Generate through that row
			thisVert = i * 16 + j;
			edge[thisVert].src = thisVert;
			edge[thisVert].dest = thisVert + 1;
			edge[thisVert].weight = rand() % 256 + 100;
		}
	}
	for (int i = 0; i < 16; i++){ //For each column
		for (int j = 0; j < 15; j++){ //Generate through that column
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

	edge[middleWalls[rand() % 7]].weight = 0;
	
	
	//Initializes all walls in maze
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			maze[i][j] = new Cell(i, j, true, true);
		}
	}
	
	//Find the walls to open and opens them
    KruskalMST(graph, maze);
}

void generateMoreThanOnePath(Cell ***maze, int wallsToKill){
    generateOnePath(maze);
    tearDownWalls(maze, wallsToKill);
}

void tearDownWalls(Cell ***maze, int wallsToKill){
    int num;
    Cell * c;
    while (wallsToKill > 0){
        num = rand() % 480;
        if (num == 0 || num == 240 || num == 127|| num == 129|| num == 112|| num == 114|| num == 367|| num == 369|| num == 352|| num == 354){
            continue;//Middle walls or walls of bottom left cell
        }
        if (num > 240){ //Top walls
            num-=240;
            c = maze[15 - num / 16][num % 16];
            if (c -> top_wall){
                c -> top_wall = true;
                wallsToKill--;
            }
        }
        else { //right walls
            c = maze[15 - num / 15][num % 15];
            if (c -> right_wall){
                c -> right_wall = false;
                wallsToKill--;
            }
        }
    }
}