ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mbed.h"
#include "maze.h"
#include "motor.h"

using namespace std;

class Controller {

public:
  int curr_x, curr_y;
  float sp; // track the initial speed of the motor

  Controller(int x, int y, float init_speed):
      x(curr_x), y(curr_y), init_speed(sp) {}
};

extern Maze *smaze;
extern Motor *cmotor(D9, D10, D11);

bool detected_walls = false;

void init_simulation();

void simulateRun();

void show_speed();

float speed_change(float curr_speed);

void decide_turn_dir();

void turnLeft();

void turnRight();
#endif
