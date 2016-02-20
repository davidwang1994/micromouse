#include "mbed.h"
#include "maze.h"
#include "pin_assignments.h"


const float SENSOR_THRESHOLD = 0.5;

//Exported from mbed for use with local Keli compiler (the mbed compiler)
//Compiling this folder with Keli allows for direct exporting onto mice

//Everything is in the same folder because I had them in that way on mbed. 
//motor/encoder/pin_assignemnt and this file are the only ones fully tested


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

/** set the walls of the current cell by using the IR sensors **/
void set_walls(int x, int y){
    if (frontIR1.read() > SENSOR_THRESHOLD && frontIR2 > SENSOR_THRESHOLD){
        // need to re-evaluate the sensor positions
        maze[y][x]->top_wall = true;
    }
    if (leftIR > SENSOR_THRESHOLD) {
        maze[y][x-1]->right_wall= true;
    }
    if (rightIR> SENSOR_THRESHOLD) {
        maze[y][x]->right_wall = true;
    }
}

int main() {
  pc.printf("\r\nHi!\r\n");
  
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
