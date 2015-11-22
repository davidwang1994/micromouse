#include "maze.h"

using namespace std;

int manhattan_dist(int x1, int x2, int y1, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int min4(int a, int b, int c, int d) {
    int min;
    (a < b) ? min = a : min = b;
    if (c < min) min = c;
    if (d < min) min = d;
    return min;
}

void init_maze() {
    int goal1 = MAZE_SIZE / 2;
    int goal2 = (MAZE_SIZE - 1) / 2;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze[i][j] = min4(manhattan_dist(i, goal1, j, goal1),
                              manhattan_dist(i, goal1, j, goal2)
                              manhattan_dist(i, goal2, j, goal1)
                              manhattan_dist(i, goal2, j, goal2));
        }
    }
}

/*
 * How each cell will sort of look printed out.
 *  +-----+ 
 *  | 100 |                                                                    
 *  +-----+  
 *
 * 4 x 4 maze example
 *   _ _ _ _ _
 *
 *   _______________________
    |                       |
 *  | 256 | 256 | 256 | 256 |
 *  |      _________________|
    |                       |
 *  | 256   256   256 | 256 |
 *  |___________       _____|
    |                       |
 *  | 256 | 256 | 256   256 |
 *  |      _____       _____|
    |                       |
 *  | 256   256   256 | 256 |
 *  |_______________________|
 *
 *
 */

void print_maze() {

    string s;
    int row_items = 6 * MAZE_SIZE + 1;
    int col_items = 3 * MAZE_SIZE + 1;

    for (int i = 0; i < col_items; i++) {
        for (int j = 0; j < row_items; j++) {
            if (i == 0 || i == col_items - 1) {
                s += "_";
            }
            if (j == 0 || j == row_items - 1) {
                s += "|";
                if (j == row_items - 1) {
                    s += "\n";
                }
            }
        }
    }
    cout << s << endl;
}

int main() {
    init_maze();
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
