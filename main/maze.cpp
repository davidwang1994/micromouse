#include "maze.h"

Cell *maze[MAZE_SIZE][MAZE_SIZE];
Cell *mazeIn[MAZE_SIZE][MAZE_SIZE];

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
    int min = UCHAR_MAX;
//    cout << "neighbors: " << endl;
//    for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); it++) {
//        cout << "y: " << (*it)->y <<  " x: " << (*it)->x << " dist: " << (*it)->dist << endl;
//    }
    for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); it++) {
//        cout << "cell x: " << (*it)->x << " y: " << (*it)->y << " min: " << (*it)->dist << endl;
        if ((*it)->dist < min) {
            min = (*it)->dist;
        }
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
            /**          cout <<"store: " << min4(manhattan_dist(i, goal1, j, goal1),
                                                       manhattan_dist(i, goal1, j, goal2),
                                                       manhattan_dist(i, goal2, j, goal1),
                                                       manhattan_dist(i, goal2, j, goal2)) <<
                                                       " in ( " << i <<" , "<< j << " )......."<<endl;
                      cout<< " ( " << i << " , " << j << " ):  " << maze[i][j]->dist << endl;
          **/
            if (i == MAZE_SIZE - 1) {
                maze[i][j]->top_wall = true;
            }
            if (j == MAZE_SIZE - 1) {
                maze[i][j]->right_wall = true;
            }
        }
    }
}

void add_cell_to_update(vector <Cell*> &stack, Cell * cell) {
    stack.push_back(cell);
}

/*
* Function to update the distances of the cells
*/
void update_distances(vector <Cell*> &stack) {
    Cell *current;
    vector<Cell*> open_neighbors;
    vector<Cell*> neighbors;

    int x, y;
    int min;
    while (!stack.empty()) {
        current = stack.back();
//        cout << "stack not empty" << endl;
//        cout << "current cell: (" << current->x << "," << current->y << ")" << endl;
        stack.pop_back();
        x = current->x;
        y = current->y;
//        cout << "update distance cell y: " << y << " x: " << x << " dist: " << current->dist << endl;
        // check top neighbor

        if (is_center(current)) {
            continue;
        }

        // check top neighbor
        if (y < MAZE_SIZE - 1) {
//            cout << "Check top neighbor" << endl;
            neighbors.push_back(maze[y + 1][x]);
            if (!current->top_wall) {
//                cout << "Has top neighbor" << endl;
                open_neighbors.push_back(maze[y + 1][x]);
            }
        }
        // check right neighbor
        if (x < MAZE_SIZE - 1) {
//            cout << "Check right neighbor" << endl;
            neighbors.push_back(maze[y][x + 1]);
            if (!current->right_wall) {
//                cout << "Has right neighbor" << endl;
                open_neighbors.push_back(maze[y][x + 1]);
            }
        }
        // check bottom neighbor
        if (y > 0) {
            neighbors.push_back(maze[y - 1][x]);
            if (!maze[y - 1][x]->top_wall) {
                open_neighbors.push_back(maze[y - 1][x]);
            }
        }
        // check left neighbor
        if (x > 0) {
            //           cout << "Check left neighbor" << endl;
            neighbors.push_back(maze[y][x - 1]);
            if (!maze[y][x - 1]->right_wall) {
//                cout << "Has left neighbor" << endl;
                open_neighbors.push_back(maze[y][x - 1]);
            }
        }
        if (open_neighbors.empty()) {
            neighbors.clear();
            continue;
        }
//        cout << "number open_neighbors: " << open_neighbors.size() << endl;
        min = min_open_neighbor(open_neighbors);
//        cout << "minimum neighbor min: " << min << endl;
        open_neighbors.clear();
        if (current->dist - 1 != min) {
//            cout << "min: " << min << endl;
            current->dist = min + 1;
            for (vector<Cell *>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
                if (!is_center(*it)) {
                    stack.push_back(*it);
                }
            }
            neighbors.clear();
        }

//        print_maze();

    }
}

void set_wall(int y, int x) {
    if (direction == TOP) {
        if (has_front_wall()) {
            maze[y][x]->top_wall = true;
        }
        if (has_right_wall()) {
            maze[y][x]->right_wall = true;
        }
        if (x > 0) {
            if (has_left_wall()) {
                maze[y][x - 1]->right_wall = true;
            }
        }
    }

    else if (direction == RIGHT) {
        if (has_front_wall()) {
            maze[y][x]->right_wall = true;
        }
        if (y > 0) {
            if (has_right_wall()) {
                maze[y - 1][x]->top_wall = true;
            }
        }
        if (has_left_wall()) {
            maze[y][x]->top_wall = true;
        }
    }

    else if (direction == DOWN) {
        if (y > 0) {
            if (has_front_wall()) {
                maze[y - 1][x]->top_wall = true;
            }
        }
        if (x > 0) {
            if (has_right_wall()) {
                maze[y][x - 1]->right_wall = true;
            }
        }
        if (has_left_wall()) {
            maze[y][x]->right_wall = true;
        }

    }

    else if (direction == LEFT) {
        if (x > 0) {
            if (has_front_wall()) {
                maze[y][x - 1]->right_wall = true;
            }
        }
        if (has_right_wall()) {
            maze[y][x]->top_wall = true;
        }
        if (y > 0) {
            if (has_left_wall()) {
                maze[y - 1][x]->top_wall = true;
            }
        }
    }
}


bool fully_explored() {
    for (int y = 0; y < MAZE_SIZE; y++) {
        for (int x = 0; x < MAZE_SIZE; x++) {
            if (maze[y][x] == false) {
                return false;
            }
        }
    }
    return true;
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
    srand((unsigned) time(0));
    //cout << time(0) << endl;
    int numCells = MAZE_SIZE * MAZE_SIZE;
    int numWalls = rand() % (numCells / 4) + (numCells * 3 / 4);
    //cout << rand() << endl;
    //cout << numWalls << endl;

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
    //cout << s << endl;
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
        //cout << "+---";
    }
    //cout << "+\n";

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
                    pc.printf("+---");
                }
                else {
                    pc.printf("+   ");
                }
                if (j == MAZE_SIZE - 1) {
                    pc.printf("+");
                }
            }

            else {
                if (j == 0) {
                    pc.printf("|");
                }

                int dist = maze[y][j]->dist;
                //      pc.printf("   ");
                if (dist > 99) {
                    pc.printf("%d", dist);
                }
                else if (dist > 9) {
                    pc.printf(" %d", dist);
                }
                else {
                    pc.printf(" %d ", dist);
                }

                if (maze[y][j]->right_wall || j == MAZE_SIZE - 1) {
                    pc.printf("|");
                }
                else {
                    pc.printf(" ");
                }
            }
        }
        pc.printf("\n");
    }

    // print bottom wall
    for (int i = 0; i < MAZE_SIZE; i++) {
        pc.printf("+---");
    }
    pc.printf("+\n\n");
}

void explore(vector<Cell*> &stack, int y, int x) {
    if (maze[y][x]->visited) {
        return;
    }
    else {
        maze[y][x]->visited = true;
        set_wall(y,x);
    }
    if (maze[y][x]->top_wall || maze[y][x]->right_wall) {
        stack.push_back(maze[y][x]);
        update_distances(stack);
    }
// if mouse is located bottom left of center then we want to prioritize exploring top/right
    if (y < MAZE_SIZE / 2 && x < MAZE_SIZE / 2) {
// explore top
        if (y < MAZE_SIZE - 1) {
            explore(stack, y + 1, x);
        }
// explore right
        if (x < MAZE_SIZE - 1) {
            explore(stack, y, x + 1);
        }
// explore down
        if (y > 0) {
            explore(stack, y - 1, x);
        }
// explore left
        if (x > 0) {
            explore(stack, y, x - 1);
        }
    }
// if mouse is located top left of center then we want to prioritize exploring bottom/right
    else if (y > MAZE_SIZE / 2 && x < MAZE_SIZE / 2) {
// explore right
        if (x < MAZE_SIZE - 1) {
            explore(stack, y, x + 1);
        }
// explore down
        if (y > 0) {
            explore(stack, y - 1, x);
        }
// explore top
        if (y < MAZE_SIZE - 1) {
            explore(stack, y + 1, x);
        }
// explore left
        if (x > 0) {
            explore(stack, y, x - 1);
        }
    }
// if mouse is located top right of center then we want to prioritize exploring bottom/left
    else if (y > MAZE_SIZE / 2 && x > MAZE_SIZE / 2) {
// explore down
        if (y > 0) {
            explore(stack, y - 1, x);
        }
// explore left
        if (x > 0) {
            explore(stack, y, x - 1);
        }
// explore top
        if (y < MAZE_SIZE - 1) {
            explore(stack, y + 1, x);
        }
// explore right
        if (x < MAZE_SIZE - 1) {
            explore(stack, y, x + 1);
        }
    }
// if mouse is located bottom right of center then we want to prioritize exploring bottom/right
    else if (y < MAZE_SIZE / 2 && x > MAZE_SIZE / 2) {
// explore left
        if (x > 0) {
            explore(stack, y, x - 1);
        }
// explore top
        if (y < MAZE_SIZE - 1) {
            explore(stack, y + 1, x);
        }
// explore down
        if (y > 0) {
            explore(stack, y - 1, x);
        }
// explore right
        if (x < MAZE_SIZE - 1) {
            explore(stack, y, x + 1);
        }
    }
}
