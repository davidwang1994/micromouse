//Signals from elsewhere:
bool UPDATE_POSITION = false;     //Needs position update when drive into new cell
bool DONE_MOVING = true;              //Driving or turning complete
bool UPDATING_INSTRUCTIONS = true;    //Control alg not yet saved an action to mouse_action

enum GlobalState {
    STARTUP,
    WAITING,
    AT_BEGINNING,
    EXPLORING_TO_CENTER,
    EXPLORING_BACK,
    AT_CENTER,
    SPEEDING_TO_CENTER,
    SPEEDING_BACK
};
GlobalState global_state;

enum MouseAction {
    DRIVE,
    TURN_RIGHT,
    TURN_LEFT,
    TURN_AROUND,
    TURN_LEFT_DIAGONAL,
    TURN_RIGHT_DIAGONAL,
    DRIVE_DIAGONAL
};
MouseAction mouse_action;
int drive_distance = 0;

enum MouseDirection {
    NORTH, SOUTH, EAST, WEST, NORTHWEST, NORTHEAST, SOUTHWEST, SOUTHEAST
};
MouseDirection mouse_direction;

//For Maze, need to make consistent
int direction = 0; //Top

#include "drive_control.h"
#include "io.h"
#include "maze.h"


Serial pc(PA_9, PA_10);  //THIS MUST GO IN MAIN

void update_position() {
    UPDATE_POSITION = false;
    //update_maze();
}

//Updates direction based on current turn action and calls function to generate next action
void update_direction() {
    switch (mouse_action) {
        case TURN_RIGHT:
            switch (mouse_direction) {
                case NORTH:
                    mouse_direction = EAST;
                    break;
                case EAST:
                    mouse_direction = SOUTH;
                    break;
                case SOUTH:
                    mouse_direction = WEST;
                    break;
                case WEST:
                    mouse_direction = NORTH;
                    break;
            }
            break;
        case TURN_LEFT:
            switch (mouse_direction) {
                case NORTH:
                    mouse_direction = WEST;
                    break;
                case EAST:
                    mouse_direction = NORTH;
                    break;
                case SOUTH:
                    mouse_direction = EAST;
                    break;
                case WEST:
                    mouse_direction = SOUTH;
                    break;
            }
            break;
        case TURN_AROUND:
            switch (mouse_direction) {
                case NORTH:
                    mouse_direction = SOUTH;
                    break;
                case EAST:
                    mouse_direction = WEST;
                    break;
                case SOUTH:
                    mouse_direction = NORTH;
                    break;
                case WEST:
                    mouse_direction = EAST;
                    break;
            }
            break;
        case TURN_RIGHT_DIAGONAL:
            switch (mouse_direction) {
                case NORTHWEST:
                    mouse_direction = NORTH;
                    break;
                case NORTH:
                    mouse_direction = NORTHEAST;
                    break;
                case NORTHEAST:
                    mouse_direction = EAST;
                    break;
                case EAST:
                    mouse_direction = SOUTHEAST;
                    break;
                case SOUTHEAST:
                    mouse_direction = SOUTH;
                    break;
                case SOUTH:
                    mouse_direction = SOUTHWEST;
                    break;
                case SOUTHWEST:
                    mouse_direction = WEST;
                    break;
                case WEST:
                    mouse_direction = NORTHWEST;
                    break;
            }
            break;
        case TURN_LEFT_DIAGONAL:
            switch (mouse_direction) {
                case NORTHWEST:
                    mouse_direction = WEST;
                    break;
                case NORTH:
                    mouse_direction = NORTHWEST;
                    break;
                case NORTHEAST:
                    mouse_direction = NORTH;
                    break;
                case EAST:
                    mouse_direction = NORTHEAST;
                    break;
                case SOUTHEAST:
                    mouse_direction = EAST;
                    break;
                case SOUTH:
                    mouse_direction = SOUTHEAST;
                    break;
                case SOUTHWEST:
                    mouse_direction = SOUTH;
                    break;
                case WEST:
                    mouse_direction = SOUTHWEST;
                    break;
            }
            break;
    }


    //update_maze_direction();
}


void setup() {
    global_state = STARTUP;
    pc.baud(9600);

    //init_maze(); //MAZE_UPDATING should be set to false every time it saved result to mouse_action or speed_action
    global_state = WAITING;
}

void waiting() {
    
}

void at_beginning() {
    if (!DONE_MOVING){
        //Wait for move to finish
    }
    
    
}

void exploring_to_center() {

    if (UPDATING_INSTRUCTIONS) {
        //Wait for instructions to finish
    }
    else if (DONE_MOVING) {
        switch (mouse_action) {
            case DRIVE:
                drive_cell();
                break;
            case TURN_RIGHT:
                turn_right();
                update_direction();
                break;
            case TURN_LEFT:
                turn_left();
                update_direction();
                break;
            case TURN_AROUND:
                turn_around();
                update_direction();
                break;
        }
    }

    if (UPDATE_POSITION) {
        update_position();
    }
}

void exploring_back() {
    if (UPDATING_INSTRUCTIONS) {
        //Wait for instructions to finish
    }
    else if (DONE_MOVING) {
        switch (mouse_action) {
            case DRIVE:
                drive_cell();
                break;
            case TURN_RIGHT:
                turn_right();
                update_direction();
                break;
            case TURN_LEFT:
                turn_left();
                update_direction();
                break;
            case TURN_AROUND:
                turn_around();
                update_direction();
                break;
        }
    }

    if (UPDATE_POSITION) {
        update_position();
    }
}

void at_center() {
    if (!DONE_MOVING){
        //Wait for move to finish
    }
}

void speeding_to_center() {
    
    
    if (UPDATING_INSTRUCTIONS) {
        //Wait for instructions to finish
    }
    else if (DONE_MOVING) {
        switch (mouse_action) {
            case DRIVE:
                speed_drive_cell(drive_distance);
                break;
            case TURN_RIGHT:
                speed_turn_right();
                update_direction();
                break;
            case TURN_LEFT:
                speed_turn_left();
                update_direction();
                break;
            case TURN_RIGHT_DIAGONAL:
                speed_turn_right_diagonal();
                update_direction();
                break;
            case TURN_LEFT_DIAGONAL:
                speed_turn_left_diagonal();
                update_direction();
                break;
            case DRIVE_DIAGONAL:
                break;//TODO
        }
    }

    if (UPDATE_POSITION) {
        update_position();
    }
}

void speeding_back() {
    if (UPDATING_INSTRUCTIONS) {
        //Wait for instructions to finish
    }
    else if (DONE_MOVING) {
        switch (mouse_action) {
            case DRIVE:
                speed_drive_cell(drive_distance);
                break;
            case TURN_RIGHT:
                speed_turn_right();
                update_direction();
                break;
            case TURN_LEFT:
                speed_turn_left();
                update_direction();
                break;
            case TURN_RIGHT_DIAGONAL:
                speed_turn_right_diagonal();
                update_direction();
                break;
            case TURN_LEFT_DIAGONAL:
                speed_turn_left_diagonal();
                update_direction();
                break;
            case DRIVE_DIAGONAL:
                break;//TODO
        }
    }

    if (UPDATE_POSITION) {
        update_position();
    }
}

int main() {

    //ticker.attach(&printStatus, 1);

    while (1) {
        switch (global_state) {
            case WAITING:
                waiting();
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
    }
}


