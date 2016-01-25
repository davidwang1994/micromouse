#include "maze.h"
#include "maze_generator.h"


Cell *maze[MAZE_SIZE][MAZE_SIZE];
Cell *mazeIn[MAZE_SIZE][MAZE_SIZE];

/*
* Calculates the total number of cells needed to get from a point (x1, y1)
* to a point (x2, y2).
*/
unsigned char manhattan_dist(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

/*
* Function that takes the minimum of the four given distances
*/
unsigned char min4(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    unsigned char min;
    (a < b) ? min = a : min = b;
    if (c < min) min = c;
    if (d < min) min = d;
    return min;
}

unsigned char min_open_neighbor(vector<Cell*> cells) {
    unsigned char min = CHAR_MAX;
    for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); it++)
        if ((*it)->dist < min) {
            min = (*it)->dist;
        }
    return min;
}

bool is_center(Cell *cell) {
    unsigned char x = cell->x;
    unsigned char y = cell->y;
    unsigned char goal1 = MAZE_SIZE / 2;
    unsigned char goal2 = (MAZE_SIZE - 1) / 2;
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
    unsigned char goal1 = MAZE_SIZE / 2;
    unsigned char goal2 = (MAZE_SIZE - 1) / 2;
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
        }
    }

//    for(int i = 0; i < MAZE_SIZE; i++){
//        for(int j = 0; j < MAZE_SIZE; j++){
//            cout<< " ( " << i << " , " << j << " ):  " << maze[i][j]->dist << endl;
//        }
//    }

    cout << "size of the matrix is: "<< sizeof(maze) << endl;
}

/*
* Initializes Cell * maze from given maze text file (formatted similar to http://www.tcp4me.com/mmr/mazes/)
* Assumes 33*33 text grid in file and only checks first 32 rows and for - | ' ' at their correct positions
* A new cell constructor was added for just posiiton and walls.
*/
void load_maze(string file_name) {
    ifstream file(file_name);
    string top, right;
    int row = MAZE_SIZE - 1;
    while (row >= 0) {
        getline(file, top);
        getline(file, right);
        for (int col = 0; col < MAZE_SIZE; col++) {
            maze[row][col] = new Cell(row, col, top[(col * 2) + 1] == '-', right[(col * 2) + 2] == '|');
        }
        row--;
    }

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

/**
* Get the currently constructed maze and write to a binary file
**/
void output_maze(){
    // output the maze to a binary file
    ofstream outfile("maze_out", ofstream::binary);
    for(int i = 0; i < MAZE_SIZE; i++){
        for(int j = 0; j < MAZE_SIZE; j++){
            outfile.write( (char*)&maze[i][j], sizeof(int) );
            outfile << maze[i][j];
        }
    }
    outfile.close();
    cout << "done..." << endl;
    cout << "list the dists of each cell below.... " << endl;
    // printing out the maze and dists to the console
//    for(int i = 0; i < MAZE_SIZE; i++){
  //      for(int j = 0; j < MAZE_SIZE; j++){
//            cout<< " ( " << i << " , " << j << " ):  " << maze[i][j]->dist << endl;
//        }
//    }
}

void serialize(int rows, int cols) {
    ofstream outfile;
    outfile.open("maze_out", ofstream::binary);
//    outfile << rows << " ";
//    outfile << cols << " ";
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
         //   outfile << maze[i][j] << " ";
            outfile.write(reinterpret_cast<char*> (&maze[i][j]), sizeof(int));
        }
    }
    outfile.close();


    /** Outputting the maze structure inti a file
    * using the fopen function
    **/
  //  FILE* pFile = fopen("maze_out", "wb");
  //  fwrite(maze, sizeof(maze), 1, pFile);
  //  fclose(pFile);
}

void deserialize() {
    int rows = 16;
    int cols = 16;
    int goal1 = MAZE_SIZE / 2;
    int goal2 = (MAZE_SIZE - 1) / 2;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            // Distance of the cell will be the minimum distance to the closest
            // one out of four middle destination cells.
            mazeIn[i][j] = new Cell(i, j, min4(manhattan_dist(i, goal1, j, goal1),
                                             manhattan_dist(i, goal1, j, goal2),
                                             manhattan_dist(i, goal2, j, goal1),
                                             manhattan_dist(i, goal2, j, goal2)));


        }
    }
    ifstream inFile;
    inFile.open("maze_out", ios::in | ios::binary);
    cout << "This line entered... " << endl;
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            inFile.read (reinterpret_cast<char*> (&mazeIn[i][j]), sizeof(int));
        }
    }
    inFile.close();

  //  FILE* pFile = fopen("maze_out", "rb");
  //  fread(mazeIn, sizeof(mazeIn), 1, pFile);
  //  fclose(pFile);
}

/**
* Read in the maze data from a previously stored binary file
**/




/*
* Initializes the default (blank maze) manhattan distance values into existing maze
* Given a 5 * 5 maze, it would look like:
*    _________________________
*   | 4  | 3  | 2  | 3  | 4  |
*   |____|____|____|____|____|
*   | 3  | 2  | 1  | 2  | 2  |
*   |____|____|____|____|____|
*   | 2  | 1  | 0  | 1  | 3  |
*   |____|____|____|____|____|
*   | 3  | 2  | 1  | 2  | 3  |
*   |____|____|____|____|____|
*   | 4  | 3  | 2  | 3  | 4  |
*   |____|____|____|____|____|
*
* Each cell stores its distance from the target cell (In this case, (2,2)  )
*/


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
//        cout << "stack not empty" << endl;
//        cout << "current cell: (" << current->x << "," << current->y << ")" << endl;
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
//            cout << "min: " << min << endl;
            current->dist = min + 1;
            for (vector<Cell *>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
                if (!is_center(*it)) {
                    stack.push_back(*it);
//                    cout << "pushed cell: (" << (*it)->x << "," << (*it)->y << ")" << endl;
                }
            }
            neighbors.clear();
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

void explore(vector<Cell*> &stack, int y, int x) {
    cout << "before check x: " << x << " y: " << y << endl;
    if (maze[y][x]->visited) {
        cout << "after check x: " << x << " y: " << y << endl;
        return;
    }
    else {
        maze[y][x]->visited = true;
    }
    if (maze[y][x]->top_wall || maze[y][x]->right_wall) {
        stack.push_back(maze[y][x]);
        update_distances(stack);
    }
    if (y > 0) {
        explore(stack, y - 1, x);
    }
    if (x > 0) {
        explore(stack, y, x - 1);
    }
    if (y < MAZE_SIZE - 1) {
        explore(stack, y + 1, x);
    }
    if (x < MAZE_SIZE - 1) {
        explore(stack, y, x + 1);
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
          //      cout << "   ";
                if (dist > 99) {
                    cout << dist;
                }
                else if (dist > 9) {
                    cout << " " << dist;
                }
                else {
                    cout << " " << dist << " ";
                }

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

void print_debug_maze() {

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
                if (mazeIn[y][j]->top_wall) {
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

                int dist = mazeIn[y][j]->dist;
                if (dist > 99) {
                    cout << dist;
                }
                else if (dist > 9) {
                    cout << " " << dist;
                }
                else {
                    cout << " " << dist << " ";
                }

                if (mazeIn[y][j]->right_wall || j == MAZE_SIZE - 1) {
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
    init_maze();
    ////    generate_random_walls();
    print_maze();

    load_maze("mazes/2008japan.maze");
    print_maze();

    vector<Cell*> cells;
    explore(cells, 0, 0);

    print_maze();
    //      load_maze("2011robotic");
    print_maze();
    serialize(16, 16);
    deserialize();
    cout << "     " << endl;
    //generateOnePath(maze);
    print_debug_maze();
}
