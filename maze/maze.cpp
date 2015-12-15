#include "maze.h"

using namespace std;

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
 * Function to update the distances of the cells
 */
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

void generateRandomWalls() {
    string s;
    srand((unsigned)time(0));
    cout << time(0) << endl;
    int numCells = MAZE_SIZE * MAZE_SIZE;
    int numWalls = rand() % (numCells/4) + (numCells * 3 / 4);
    cout << rand() << endl;
    cout << numWalls << endl;

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            int y = MAZE_SIZE -  1  - i;
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
 *     3    |     |_____|___________|
            |                 |     |
 *          | 256   256   256 | 256 | (2,3)
 *  i  6    |___________      |_____|
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
 */

/*
 * Function to print out an ascii representation of the maze.
 */
void print_maze() {

    string s;
    int row_items = 3 * MAZE_SIZE + 1;
    int col_items = 6 * MAZE_SIZE + 1;
    bool numberRow = false;
    bool numberPrinted = false;
    int prevNumRow = 2;

    for (int i = 0; i < row_items; i++) {


        // numberRow is set to true when i is equal to a row number that we
        // want to place the cells distance in
        if (i == prevNumRow) {
            numberRow = true;
            prevNumRow += 3;
        }
        else {
            numberRow = false;
        }

        for (int j = 0; j < col_items; j++) {
            int y = i;
            if (i > 0) {
//             y = MAZE_SIZE - 1 - ((i-1) / 3);
             y = MAZE_SIZE - 1 - (i/3);
            }

            int x;
            (j < col_items - 1) ? x = j / 6 : x = j / 7;
            bool right_wall = false;
            bool top_wall = false;
            if ((j+6) % 6 == 0) {
                 right_wall = maze[y][x]->right_wall;
            }
            if ((i+3) % 3 == 0) {
                top_wall = maze[y][x]->top_wall;
            }


            // Print out the maze borders
            if (j != 0 && j != col_items - 1 && (i == 0 || !right_wall) && (i == 0 || i == row_items - 1)) {
                s += "_";
            }
            else if (i != 0 && ((right_wall && (j % 6) == 0) || ((j == 0 || j == col_items - 1)))) {
                s += "|";
            }

            else if (top_wall) {
                s += "___";
                j += 2;
            }

            else {
                int scaled_j = j % 6;
                if (numberRow && !numberPrinted && scaled_j > 1 && scaled_j < 5) {
                    int dist = maze[y][x]->dist;
                    if (dist > 99) {
                        s += to_string(dist);
                        j += 2;
                        numberPrinted = true;
                    }
                    else if (dist > 9 && scaled_j > 2) {
                        s += to_string(dist);
                        j += 1;
                        numberPrinted = true;
                    }
                    else if (dist < 10 && scaled_j > 2) {
                        s += to_string(dist);
                        numberPrinted = true;
                    }
                    // Handles the bug of not printing a space before a single
                    // or double digit number.
                    else {
                        s += " ";
                    }
                }
                else {
                    if ((scaled_j) > 4) {
                        numberPrinted = false;
                    }
                    s += " ";
                }
            }
        }
        s += "\n";
    }
    cout << s << endl;
}


int main() {
    init_maze();
//    generateRandomWalls();
    print_maze();
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
