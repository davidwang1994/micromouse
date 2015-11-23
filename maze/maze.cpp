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
            maze[i][j] = new Cell(min4(manhattan_dist(i, goal1, j, goal1),
                              manhattan_dist(i, goal1, j, goal2),
                              manhattan_dist(i, goal2, j, goal1),
                              manhattan_dist(i, goal2, j, goal2)));
        }
    }
}

void update_distances(vector<Cell*> &stack) {
    Cell *current, *top, *right, *bottom, *left;
    int neighbor_dist[4];
    int x, y;
    int min;
    while (!stack.empty()) {
        current = stack.back();
        stack.pop_back();
        x = current->x;
        y = current->y;
        if (y < MAZE_SIZE - 1)
            top = maze[x][y+1];
        if (x < MAZE_SIZE - 1)
            right = maze[x+1][y];
        if (y > 0)
            bottom = maze[x][y-1];
        if (x > 0)
            left = maze[x-1][y];
        min = min4(top->dist, right->dist, bottom->dist, left->dist);
        if (current->dist - 1 != min) {
            current->dist = min + 1;
            stack.push_back(top);
            stack.push_back(right);
            stack.push_back(bottom);
            stack.push_back(left);
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
