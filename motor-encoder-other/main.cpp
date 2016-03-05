#include "mbed.h"
<<<<<<< HEAD
#include "drive_control.h"
=======
#include "ir.h"
#include "../maze/maze.h"
#include "pin_assignments.h"

>>>>>>> 110ec1ffbc98eaa40298e8309535cf94c691c1a9


//Exported from mbed for use with local Keli compiler (the mbed compiler)
//Compiling this folder with Keli allows for direct exporting onto mice

//Everything is in the same folder because I had them in that way on mbed. 
//motor/encoder/pin_assignemnt and this file are the only ones fully tested


<<<<<<< HEAD
DigitalIn user_button(USER_BUTTON);
Serial pc(PA_2, PA_3); //D0, D1

InterruptIn leftChannelA(D3);
InterruptIn rightChannelA(D5);

Ticker ticker;

=======
>>>>>>> 110ec1ffbc98eaa40298e8309535cf94c691c1a9
void leftGotPulse(){
    pc.printf("Left: %i\r\n", leftEncoder);
}

void rightGotPulse(){
    pc.printf("Right: %i\r\n", rightEncoder);
}

float speed;

void printStatus(){
    pc.printf("\r\nLeft: %i\r\n", leftEncoder);
    pc.printf("Right: %i\r\n", rightEncoder);
    pc.printf("Motor Speed: %.2f\r\n", speed);
}

void explore(vector<Cell*> &stack, int y, int x) {
    if (maze[y][x]->visited) {
        return;
    }
    else {
        maze[y][x]->visited = true;
        set_walls(y, x);
    }
    if (maze[y][x]->top_wall || maze[y][x]->right_wall) {
        stack.push_back(maze[y][x]);
        update_distances(stack);
    }

    // if mouse is located bottom left of center then we want to prioritize exploring top/right
    if (y < MAZE_SIZE / 2 && x < MAZE_SIZE / 2) {
        // explore top
        if (y < MAZE_SIZE - 1) {
            direction = TOP;
            explore(stack, y + 1, x);
        }
        // explore right
        if (x < MAZE_SIZE - 1) {
            direction = RIGHT;
            explore(stack, y, x + 1);
        }
        // explore down
        if (y > 0) {
            direction = DOWN;
            explore(stack, y - 1, x);
        }
        // explore left
        if (x > 0) {
            direction = LEFT;
            explore(stack, y, x - 1);
        }
    }

        // if mouse is located top left of center then we want to prioritize exploring bottom/right
    else if (y > MAZE_SIZE / 2 && x < MAZE_SIZE / 2) {
        // explore right
        if (x < MAZE_SIZE - 1) {
            direction = RIGHT;
            explore(stack, y, x + 1);
        }
        // explore down
        if (y > 0) {
            direction = DOWN;
            explore(stack, y - 1, x);
        }
        // explore top
        if (y < MAZE_SIZE - 1) {
            direction = TOP;
            explore(stack, y + 1, x);
        }
        // explore left
        if (x > 0) {
            direction = LEFT;
            explore(stack, y, x - 1);
        }
    }

        // if mouse is located top right of center then we want to prioritize exploring bottom/left
    else if (y > MAZE_SIZE / 2 && x > MAZE_SIZE / 2) {
        // explore down
        if (y > 0) {
            direction = DOWN;
            explore(stack, y - 1, x);
        }
        // explore left
        if (x > 0) {
            direction = LEFT;
            explore(stack, y, x - 1);
        }
        // explore top
        if (y < MAZE_SIZE - 1) {
            direction = TOP;
            explore(stack, y + 1, x);
        }
        // explore right
        if (x < MAZE_SIZE - 1) {
            direction = RIGHT;
            explore(stack, y, x + 1);
        }
    }

        // if mouse is located bottom right of center then we want to prioritize exploring bottom/right
    else if (y < MAZE_SIZE / 2 && x > MAZE_SIZE / 2) {
        // explore left
        if (x > 0) {
            direction = LEFT;
            explore(stack, y, x - 1);
        }
        // explore top
        if (y < MAZE_SIZE - 1) {
            direction = TOP;
            explore(stack, y + 1, x);
        }
        // explore down
        if (y > 0) {
            direction = DOWN;
            explore(stack, y - 1, x);
        }
        // explore right
        if (x < MAZE_SIZE - 1) {
            direction = RIGHT;
            explore(stack, y, x + 1);
        }
    }
}

int main() {
  pc.printf("\r\nHi! Built with keil\r\n");
  
  //leftChannelA.rise(&leftGotPulse);
  //rightChannelA.rise(&rightGotPulse);
  
  bool pressed = false;
  int motorState = 0;
  speed = 0.0f;
  
  ticker.attach(&printStatus, 0.5);
  
  while(1){
    if (user_button == 0 && !pressed){
        pressed = true;
        
        
        switch (motorState){
            case 0: speed = 0.0f; break;
            case 1: speed = 0.1f; break;
            case 2: speed = 0.5f; break;
            case 3: speed = 1.0f; break;
            case 4: speed = 0.0f; break;
            case 5: speed = -0.1f; break;
            case 6: speed = -0.5f; break;
            case 7: speed = -1.0f; motorState = -1; break;
        }
        motorState++;
        drive(speed);
    }
    else if (user_button == 1){
        pressed = false;
    }
  }
}
