#include "maze.h"
#include <iostream>
#include <fstream>

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
    int s = 0;
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
/*
void exploring_back(){
   if (UPDATE_POSITION) {
   //    update_position();
   }

   if (UPDATING_INSTRUCTIONS) {
       //Wait for instructions to finish
   }
   else if (DONE_MOVING) {
       UPDATE_FINISHED = false;
       switch (mouse_action) {
           case DRIVE:
               drive_cell();
               break;
           case TURN_RIGHT:
               turn_right(drive_cell);
               break;
           case TURN_LEFT:
               turn_left(drive_cell);
               break;
           case TURN_AROUND:
               turn_around(drive_cell);
               break;
       }
   }
}*/

void load_ref_maze(string file_name) {
    ifstream file(file_name);
    string top, right;
    int row = MAZE_SIZE - 1;
    while (row >= 0) {
        getline(file, top);
        getline(file, right);
        for (int col = 0; col < MAZE_SIZE; col++) {
            mazeIn[row][col] = new Cell(row, col, top[(col * 2) + 1] == '-', right[(col * 2) + 2] == '|');
        }
        row--;
    }

    int goal1 = MAZE_SIZE / 2;
    int goal2 = (MAZE_SIZE - 1) / 2;
    int s = 0;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            // Distance of the cell will be the minimum distance to the closest
            // one out of four middle destination cells.
            //      maze[i][j]-> dist = s;
            //      s++;
            mazeIn[i][j]->dist = min4(manhattan_dist(i, goal1, j, goal1),
                                      manhattan_dist(i, goal1, j, goal2),
                                      manhattan_dist(i, goal2, j, goal1),
                                      manhattan_dist(i, goal2, j, goal2));
        }
    }
}




Cell* min_open_neighbor(vector<Cell*> cells) {
    int min = UCHAR_MAX;
    Cell *min_cell;
//    cout << "neighbors: " << endl;
//    for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); it++) {
//        cout << "y: " << (*it)->y <<  " x: " << (*it)->x << " dist: " << (*it)->dist << endl;
//    }
    for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); it++) {
//        cout << "cell x: " << (*it)->x << " y: " << (*it)->y << " min: " << (*it)->dist << endl;
        if ((*it)->dist < min) {
            min = (*it)->dist;
            min_cell = *it;
        }
    }
    return min_cell;
}

vector<Cell*> all_min_open_neighbors(vector<Cell*> cells) {
    int min = UCHAR_MAX;
    vector<Cell*> min_open_neighbors;
//    cout << "neighbors: " << endl;
//    for (vector<Cell *>::iterator it = cells.begin(); it != cells.end(); it++) {
//        cout << "y: " << (*it)->y <<  " x: " << (*it)->x << " dist: " << (*it)->dist << endl;
//    }
    for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); it++) {
//        cout << "cell x: " << (*it)->x << " y: " << (*it)->y << " min: " << (*it)->dist << endl;
        if ((*it)->dist < min) {
            min = (*it)->dist;
        }
    }

    for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); it++) {
        if ((*it)->dist == min) {
            min_open_neighbors.push_back(*it);
        }
    }

    return min_open_neighbors;
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
            /*        if (i == MAZE_SIZE - 1) {
                        maze[i][j]->top_wall = true;
                        std::cout << "Setting top wall at: " << std::endl;
                        std::cout << "pos: [ " << i << " , " << j << " ] " << std::endl;
                    }
                    if (j == MAZE_SIZE - 1) {
                        std::cout << "Setting right wall at: " << std::endl;
                        std::cout << "pos: [ " << i << " , " << j << " ] " << std::endl;
                        maze[i][j]->right_wall = true;
                    }
            */
        }
    }
}

void add_cell_to_update(vector <Cell*> &stack, Cell * cell) {
    stack.push_back(cell);
}

/* Determine the nearby cell with the smallest distance  
 *
 */
static Cell* next_move(Cell *current){
    vector<Cell*> open_neighbors;
    vector<Cell*> min_neighbors;
    Cell *min_neighbor;
    int next_dir;
    int x, y, _minX, _minY, i, j, nextX, nextY;
    y = current->y;
    x = current->x;

    /*
    // the ordering is maze[y][x]
    // center cell indices of maze
    int x_goal = MAZE_SIZE / 2;
    int y_goal = (MAZE_SIZE - 1) / 2;
    // current cell indices
    // initial pos of next cell.
    _minY = y;
    _minX = x;
    //  print_maze();
*/
    // test printing
/*    if(current_direction == TOP) std::cout << "Mouse is facing up... " << std::endl;
    if(current_direction == DOWN) std::cout << "Mouse is facing down... " << std::endl;
    if(current_direction == LEFT) std::cout << "Mouse is facing left... " << std::endl;
    if(current_direction == RIGHT) std::cout << "Mouse is facing right... " << std::endl;
*/
    //   std::cout << "Mouse is at :  [ x: " << y << " , y: " << x << " ]" << "with dist: " << current->dist << std::endl;
    // checks the next cell with the smallest distance

    if (y < MAZE_SIZE - 1 && !maze[y][x]->top_wall) {
        open_neighbors.push_back(maze[y+1][x]);
    }
    if (x < MAZE_SIZE - 1 && !maze[y][x]->right_wall) {
        open_neighbors.push_back(maze[y][x+1]);
    }
    if (x > 0 && !maze[y][x-1]->right_wall) {
        open_neighbors.push_back(maze[y][x-1]);
    }
    if (y > 0 && !maze[y-1][x]->top_wall) {
        open_neighbors.push_back(maze[y - 1][x]);
    }
    min_neighbors = all_min_open_neighbors(open_neighbors);

    min_neighbor = min_neighbors.back();

    for (vector<Cell*>::iterator it = min_neighbors.begin(); it != min_neighbors.end(); it++) {

        _minX = (*it)->x;
        _minY = (*it)->y;

        if((_minX < x) && (_minY == y)){
            next_dir = LEFT;
            cout << "--> Turning LEFT!!" << endl;
        }

        else if((_minX == x) && (_minY > y)){
            next_dir = TOP;
            cout << "--> Turning TOP!!" << endl;
        }

        else if((_minX > x) && (_minY == y)){
            next_dir = RIGHT;
            cout << "--> Turning RIGHT!!" << endl;
        }

        else if((_minX < x) && (_minY < y)){
            next_dir = DOWN;
            cout << "--> Turning DOWN!!" << endl;
        }

        if (next_dir == current_direction) {
            min_neighbor = (*it);
            break;
        }

    }

    next_direction = next_dir;
    current_direction = next_direction;

    nextX = min_neighbor->x;
    nextY = min_neighbor->y;

    return min_neighbor;

/*
    for(i = -1; i <= 1; i++){
        for(j = -1; j <= 1; j++){

            int next_x = x + i;
            int next_y = y + j;
            if(i == 0 && j == 0) continue;
            // border cells are ignored
            if(next_x < 0 || next_y < 0)  continue;
            // means cell does not exist
            if(maze[next_y][next_x] == 0) continue;
            // ignore diagnal cells for now
            if((next_x != current->x) && (next_y != current->y))  continue;
            // if the next cell has a smaller distance then check
            if(maze[next_y][next_x]->dist < current->dist){
                // breaking ties
                if((next_x == _minX) && (next_y == _minY)){
                    // check if the mouse is facing straight 0->1->2->3 counter-clockwise

                    if (current_direction == TOP) {
                        if (  maze[y][x] -> top_wall  ) {
                            maze[y][x]->top_wall = true;
                        }
                        if (  maze[y][x] -> right_wall  ) {
                            maze[y][x]->right_wall = true;
                        }
                        if (x > 0) {
                            if (  maze[y][x - 1]->right_wall  ) {
                                maze[y][x - 1]->right_wall = true;
                            }
                        }
                        //   std::cout << "..." << std::endl;
                    }

                    else if (current_direction == RIGHT) {
                        if (  maze[y][x]->right_wall  ) {
                            maze[y][x]->right_wall = true;
                        }
                        if (y > 0) {
                            if (  maze[y - 1][x]->top_wall  ) {
                                maze[y - 1][x]->top_wall = true;
                            }
                        }
                        if (  maze[y][x]->top_wall  ) {
                            maze[y][x]->top_wall = true;
                        }
                        //    std::cout << "......." << std::endl;
                    }

                    else if (current_direction == DOWN) {
                        if (y > 0) {
                            if (  maze[y - 1][x]->top_wall  ) {
                                maze[y - 1][x]->top_wall = true;
                            }
                        }
                        if (x > 0) {
                            if (  maze[y][x - 1]->right_wall  ) {
                                maze[y][x - 1]->right_wall = true;
                            }
                        }
                        if (  maze[y][x]->right_wall  ) {
                            maze[y][x]->right_wall = true;
                        }
                        //   std::cout << ".................." << std::endl;

                    }

                    else if (current_direction == LEFT) {
                        if (x > 0) {
                            if (  maze[y][x - 1]->right_wall  ) {
                                maze[y][x - 1]->right_wall = true;
                            }
                        }
                        if (  maze[y][x]->top_wall  ) {
                            maze[y][x]->top_wall = true;
                        }
                        if (y > 0) {
                            if (  maze[y - 1][x]->top_wall  ) {
                                maze[y - 1][x]->top_wall = true;
                            }
                        }
                        //    std::cout << "....,,,,,,,........." << std::endl;
                    }


                    if((current_direction == TOP && next_y > y) || (current_direction == DOWN && next_y < y) ||
                       (current_direction == LEFT && next_x < x) || (current_direction == RIGHT && next_x > x)){
                        _minX = x + i;
                        _minY = y + j;
                        continue;
                    }
                        // check for whichever is closer to the center of the maze
                    else{
                        float prev_to_goal = sqrt(pow(x_goal - _minX, 2.0) + pow(y_goal - _minY, 2.0));
                        float curr_to_goal = sqrt(pow(x_goal - next_x, 2.0) + pow(y_goal - next_y, 2.0));
                        if(curr_to_goal < prev_to_goal){
                            _minX = x + i;
                            _minY = y + j;
                            continue;
                        }
                    }
                }
                else{
                    // not tie condition
                    _minX = x + i;
                    _minY = y + j;
                }
            }

        }
    }

    if(_minX == x && _minY == y){
        std::cout << "cell distances not updated correctly... [current is smallest]" << std::endl;
    }
    // signals next cell's direction
    if((_minX < x) && (_minY == y)){
        next_cell_direction = LEFT;
        cout << "--> Turning LEFT!!" << endl;
        current_direction = LEFT;
    }

    else if((_minX == x) && (_minY > y)){
        next_cell_direction = TOP;
        cout << "--> Turning TOP!!" << endl;
        current_direction = TOP;
    }

    else if((_minX > x) && (_minY == y)){
        next_cell_direction = RIGHT;

        cout << "--> Turning RIGHT!!" << endl;
        current_direction = RIGHT;
    }

    else if((_minX < x) && (_minY < y)){
        next_cell_direction = DOWN;
        cout << "--> Turning DOWN!!" << endl;
        current_direction = DOWN;
    }
    std::cout << "next_cell's position: [ " << _minY << " , " << _minX << " ] " << "at dist: " << maze[_minY][_minX]->dist << std::endl;
    return maze[_minY][_minX];
    */
}



/*
* Function to update the distances of the cells
*/
static void update_distances(vector <Cell*> &stack) {
    Cell *current;
    vector<Cell*> open_neighbors;
    vector<Cell*> neighbors;
    int x, y;
    int min;
    while (!stack.empty()) {
        current = stack.back();
        stack.pop_back();
        x = current->x;
        y = current->y;

        if (is_center(current)) {
            std::cout << "*** center found! ***" << std::endl;
            continue;
        }

        // check top neighbor
        if (y < MAZE_SIZE - 1) {
            neighbors.push_back(maze[y + 1][x]);
            if (!current->top_wall) {
                open_neighbors.push_back(maze[y + 1][x]);
            }
        }
        // check right neighbor
        if (x < MAZE_SIZE - 1) {
            neighbors.push_back(maze[y][x + 1]);
            if (!current->right_wall) {
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
        min = min_open_neighbor(open_neighbors)->dist;
//        cout << "minimum neighbor min: " << min << endl;
        open_neighbors.clear();
        if (current->dist - 1 != min) {
            current->dist = min + 1;
            for (vector<Cell *>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
                if (!is_center(*it)) {
                    stack.push_back(*it);
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
}

void print_maze() {

    // print top wall
    for (int i = 0; i < MAZE_SIZE; i++) {
        std::cout << "+---";
    }
    std::cout << "+\n";

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
                    printf("+---");
                }
                else {
                    printf("+   ");
                }
                if (j == MAZE_SIZE - 1) {
                    printf("+");
                }
            }

            else {
                if (j == 0) {
                    printf("|");
                }

                int dist = maze[y][j]->dist;
                //      pc.printf("   ");
                if (dist > 99) {
                    printf("%d", dist);
                }
                else if (dist > 9) {
                    printf(" %d", dist);
                }
                else {
                    printf(" %d ", dist);
                }

                if (maze[y][j]->right_wall || j == MAZE_SIZE - 1) {
                    printf("|");
                }
                else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    // print bottom wall
    for (int i = 0; i < MAZE_SIZE; i++) {
        printf("+---");
    }
    printf("+\n\n");
}


void print_ref_maze() {

    // print top wall
    for (int i = 0; i < MAZE_SIZE; i++) {
        std::cout << "+---";
    }
    std::cout << "+\n";

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
                    printf("+---");
                }
                else {
                    printf("+   ");
                }
                if (j == MAZE_SIZE - 1) {
                    printf("+");
                }
            }

            else {
                if (j == 0) {
                    printf("|");
                }

                int dist = mazeIn[y][j]->dist;
                //      pc.printf("   ");
                if (dist > 99) {
                    printf("%d", dist);
                }
                else if (dist > 9) {
                    printf(" %d", dist);
                }
                else {
                    printf(" %d ", dist);
                }

                if (mazeIn[y][j]->right_wall || j == MAZE_SIZE - 1) {
                    printf("|");
                }
                else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    // print bottom wall
    for (int i = 0; i < MAZE_SIZE; i++) {
        printf("+---");
    }
    printf("+\n\n");
}


void find_path(vector<Cell*> &stack, int y, int x) {
if (maze[y][x]->visited) {
return;
}
else {
maze[y][x]->visited = true;
//   set_wall(y,x);
}


if (maze[y][x]->top_wall || maze[y][x]->right_wall) {
stack.push_back(maze[y][x]);
update_distances(stack);
}
Cell * curr = next_move(maze[y][x]);
find_path(stack, curr->y, curr->x);
}



//Called by maze after computing
//Uses current direction and direction of next cell to compute the next move
/*
void get_next_move(){
    
    switch(next_direction){
        case IS_AT_BEGINNING: 
            arrived_at_beginning(); //Blocking this main thread and set status to WAITING in meantime. Drive can finish
        break;
        case IS_AT_CENTER: 
            arrived_at_center(); //Blocking this main thread and set status to WAITING in meantime. Drive can finish.
        break;
        case NORTH: 
            switch (current_direction) {
                case NORTH: mouse_action = DRIVE;
                case WEST: mouse_action = TURN_RIGHT;
                case SOUTH: mouse_action = TURN_AROUND;
                case EAST: mouse_action = TURN_LEFT;
            }
            break;
        case WEST: 
            switch (current_direction) {
                case NORTH: mouse_action = TURN_LEFT;
                case WEST: mouse_action = DRIVE;
                case SOUTH: mouse_action = TURN_RIGHT;
                case EAST: mouse_action = TURN_AROUND;
            }
            break;
        case SOUTH: 
            switch (current_direction) {
                case NORTH: mouse_action = TURN_AROUND;
                case WEST: mouse_action = TURN_LEFT;
                case SOUTH: mouse_action = DRIVE;
                case EAST: mouse_action = TURN_RIGHT;
            }
            break;
        case EAST: 
            switch (current_direction) {
                case NORTH: mouse_action = TURN_RIGHT;
                case WEST: mouse_action = TURN_AROUND;
                case SOUTH: mouse_action = TURN_LEFT;
                case EAST: mouse_action = DRIVE;
            }
            break;
    }
    UPDATING_INSTRUCTIONS = false;
}*/



static void set_wall(int y, int x) {
    if (current_direction == TOP) {
        if (  mazeIn[y][x] -> top_wall  ) {
            maze[y][x]->top_wall = true;
        }
        if (  mazeIn[y][x] -> right_wall  ) {
            maze[y][x]->right_wall = true;
        }
        if (x > 0) {
            if (  mazeIn[y][x - 1]->right_wall  ) {
                maze[y][x - 1]->right_wall = true;
            }
        }
        //   std::cout << "..." << std::endl;
    }

    else if (current_direction == RIGHT) {
        if (  mazeIn[y][x]->right_wall  ) {
            maze[y][x]->right_wall = true;
        }
        if (y > 0) {
            if (  mazeIn[y - 1][x]->top_wall  ) {
                maze[y - 1][x]->top_wall = true;
            }
        }
        if (  mazeIn[y][x]->top_wall  ) {
            maze[y][x]->top_wall = true;
        }
        //    std::cout << "......." << std::endl;
    }

    else if (current_direction == DOWN) {
        if (y > 0) {
            if (  mazeIn[y - 1][x]->top_wall  ) {
                maze[y - 1][x]->top_wall = true;
            }
        }
        if (x > 0) {
            if (  mazeIn[y][x - 1]->right_wall  ) {
                maze[y][x - 1]->right_wall = true;
            }
        }
        if (  mazeIn[y][x]->right_wall  ) {
            maze[y][x]->right_wall = true;
        }
        //   std::cout << ".................." << std::endl;

    }

    else if (current_direction == LEFT) {
        if (x > 0) {
            if (  mazeIn[y][x - 1]->right_wall  ) {
                maze[y][x - 1]->right_wall = true;
            }
        }
        if (  mazeIn[y][x]->top_wall  ) {
            maze[y][x]->top_wall = true;
        }
        if (y > 0) {
            if (  mazeIn[y - 1][x]->top_wall  ) {
                maze[y - 1][x]->top_wall = true;
            }
        }
        //    std::cout << "....,,,,,,,........." << std::endl;
    }
}

void explore(vector<Cell*> &stack, int y, int x) {
//    if (direction == 9) {
//        return;
//    }
    cout << "explore y: " << y << " x: " << x << endl;
//    maze[y][x]->visited = true;
    set_wall(y,x);

    if (maze[y][x]->top_wall || maze[y][x]->right_wall) {
        update_distances(stack);
    }
    Cell * curr = next_move(maze[y][x]);

//    if (is_center(curr)) {
//        direction = 9;
//    }
//    explore(stack, curr->y, curr->x);
}


void explore1(vector<Cell*> &stack, int y, int x) {
if (maze[y][x]->visited) {
    return;
}
else {
    maze[y][x]->visited = true;
    set_wall(y,x);
}
if (maze[y][x]->top_wall || maze[y][x]->right_wall) {
    stack.push_back(maze[y][x]);    //COMPILE ERROR
    update_distances(stack);        //COMPILE ERROR
}
// if mouse is located bottom left of center then we want to prioritize exploring top/right
if (y < MAZE_SIZE / 2 && x < MAZE_SIZE / 2) {
// explore top
    if (y < MAZE_SIZE - 1) {
current_direction = TOP;
        explore(stack, y + 1, x);
    }
// explore right
    if (x < MAZE_SIZE - 1) {
current_direction = RIGHT;
        explore(stack, y, x + 1);
    }
// explore down
    if (y > 0) {
current_direction = DOWN;
        explore(stack, y - 1, x);
    }
// explore left
    if (x > 0) {
current_direction = LEFT;
        explore(stack, y, x - 1);
    }
}
// if mouse is located top left of center then we want to prioritize exploring bottom/right
else if (y > MAZE_SIZE / 2 && x < MAZE_SIZE / 2) {
// explore right
    if (x < MAZE_SIZE - 1) {
current_direction = RIGHT;
        explore(stack, y, x + 1);
    }
// explore down
    if (y > 0) {
current_direction = DOWN;
        explore(stack, y - 1, x);
    }
// explore top
    if (y < MAZE_SIZE - 1) {
current_direction = TOP;
        explore(stack, y + 1, x);
    }
// explore left
    if (x > 0) {
current_direction = LEFT;
        explore(stack, y, x - 1);
    }
}
// if mouse is located top right of center then we want to prioritize exploring bottom/left
else if (y > MAZE_SIZE / 2 && x > MAZE_SIZE / 2) {
// explore down
    if (y > 0) {
current_direction = DOWN;
        explore(stack, y - 1, x);
    }
// explore left
    if (x > 0) {
current_direction = LEFT;
        explore(stack, y, x - 1);
    }
// explore top
    if (y < MAZE_SIZE - 1) {
current_direction = TOP;
        explore(stack, y + 1, x);
    }
// explore right
    if (x < MAZE_SIZE - 1) {
current_direction = RIGHT;
        explore(stack, y, x + 1);
    }
}
// if mouse is located bottom right of center then we want to prioritize exploring bottom/right
else if (y < MAZE_SIZE / 2 && x > MAZE_SIZE / 2) {
// explore left
    if (x > 0) {
current_direction = LEFT;
        explore(stack, y, x - 1);
    }
// explore top
    if (y < MAZE_SIZE - 1) {
current_direction = TOP;
        explore(stack, y + 1, x);
    }
// explore down
    if (y > 0) {
current_direction = DOWN;
        explore(stack, y - 1, x);
    }
// explore right
    if (x < MAZE_SIZE - 1) {
current_direction = RIGHT;
        explore(stack, y, x + 1);
    }
}
}

void test_set_wall(){
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            set_wall(i, j);
        }
    }
}

void update_position() {
    UPDATE_POSITION = false;
    UPDATE_FINISHED = false;
    // update to next cell first


    // set walls

    // then call update_distance()


    UPDATE_FINISHED = true;
}





int main() {
    load_ref_maze("test2.maze");
    print_ref_maze();

    init_maze();
    //   test_set_wall();
    print_maze();


    std::vector <Cell*> vc;
    vc.push_back(maze[0][0]);
    explore(vc, 0, 0);
    print_maze();
//    explore(vc, 0, 0);
//    print_maze();
}
