#include "testController.h"
//#include "mbed.h"

Maze *smaze;
Motor *cmotor;
float curr_speed;
/*
* Initialize on the maze object to be 5 by 5
* that will later be used to simulate the run
*/
void init_simulation(){
  smaze->init_maze();
  smaze->load_maze("mazes/2008japan.maze");
  print_maze(); // test print the maze
}
/*
* Simuate a run on the test maze that is
* 5 by 5.
*/
void simulateRun(){
  vector<Cell*> cells;
  smaze->explore(cells, 0, 0){
    // drives the motor forward by a arbitrary distance
    cmotor.drive(1.0f);
    // check if has walls
    if(top_wall){
      cout << "Top wall detected! " << endl;
      cmotor->turn(1);
      curr->speed = 0.2f;
    }
    if(right_wall){
      cout << "Right wall detected! " << endl;
      cmotor->turn(1);
      curr->speed = 0.5f;
    }
  }
}

/*
* print out the current speed that the
* motor is driving at.
*/
void show_speed(){
    cout << "Current motor speed is: " << curr_speed << endl;
}

int main(){
  // initialize the motor and maze objects
  init_simulation();
  simulateRun();
}
