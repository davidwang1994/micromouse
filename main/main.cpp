#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define NORTH_WEST 4
#define NORTH_EAST 5
#define SOUTH_WEST 6
#define SOUTH_EAST 7
#define IS_AT_BEGINNING 8
#define IS_AT_CENTER 9


//Internal for elsewhere
int direction = NORTH; //maze internal

//Signals from/to elsewhere:
int current_direction = NORTH;
int next_direction = -1; //A direction or AT_BEGINNING or AT_CENTER
int drive_distance = 1; //MAZE MUST SPECIFY DISTANCE TO DRIVE IN SPEED DRIVE MODE

int mouse_x = 0;
int mouse_y = 0;

float drive_top_speed = 0.1;
float turn_top_speed = 0.2;

bool has_left_wall = true;
bool has_right_wall = true;

bool UPDATE_POSITION = false;     //Needs position update when drive into new cell
bool DONE_MOVING = true;              //Driving or turning complete
bool UPDATING_INSTRUCTIONS = true;    //Control alg not yet saved an action to mouse_action
bool UPDATE_FINISHED = false;       //Contorl alg has saved next action to mouse_action. Will always be false unless just finshed updating and next action has not been used yet

bool LAST_ACTION_WAS_DRIVE = false;



enum MouseMode {
    EXPLORE,
    SPEED_RUN
};
MouseMode mouse_mode = EXPLORE;

enum GlobalState {
    STARTUP,
    WAITING,
    AT_BEGINNING,
    EXPLORING_TO_CENTER,
    EXPLORING_BACK,
    AT_CENTER,
    SPEEDING_TO_CENTER,
    SPEEDING_BACK,
    LOST
};
GlobalState global_state;

enum MouseAction {
    DRIVE,
    TURN_RIGHT,
    TURN_LEFT,
    TURN_AROUND,
    TURN_LEFT_DIAGONAL,
    TURN_RIGHT_DIAGONAL,
    DRIVE_DIAGONAL,
    ARRIVED_AT_CENTER, 
    ARRIVED_AT_BEGINNING
};
MouseAction mouse_action;


#include "maze.h"
#include "drive_control.h"
#include "io.h"
#include "mouse_control.h"

Serial pc(PA_9, PA_10);  //THIS MUST GO IN MAIN
//Serial pc(D1, D0);





Ticker ticker;
Ticker ticker2;
void check(){
    ledGreen = 1;
}
int main() {
    print_battery();

    //gyro.enable();

    
    //wait(1.5);
    
    //print_gyro();
    
    //wait(1.5);
    
    //print_gyro();
    
    
    //ticker.attach(&print_gyro, 1);
    
    //InterruptIn pb3(PB_3);
    
    //drive(0.01);
    
    //wait(2);
    //drive(0);
    
    //pb3.rise(&check);
    
    
    
    
    
/*
    setup();
    
    wait(0.3);
    //print_ir();
    ticker.attach(&print_encoder, 1);
    
    
    while (userButton);
    
    wait(2);
    
    global_state = LOST;
    int count = 1;
    DONE_MOVING = false;
    //drive_cell();
   
    turn_right(&drive_cell);
    */
    
while(1){
    ledGreen = user_button;
}
    
    /*while (1) {
        switch (global_state) {
            case LOST: 
                lost();
                break;
            case WAITING:
                //Does nothing and waits for something else to finish
                break;
            case AT_BEGINNING:
                at_beginning();
                break;
            case EXPLORING_TO_CENTER:
                exploring_to_center();
                break;
            case EXPLORING_BACK:
                exploring_back();
                break;
            case AT_CENTER:
                at_center();
                break;
            case SPEEDING_TO_CENTER:
                speeding_to_center();
                break;
            case SPEEDING_BACK:
                speeding_back();
                break;
            default:
                break;
                //Do nothing while in STARTUP
        }
    }*/
}
