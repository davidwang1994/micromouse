#ifndef MOUSE_CONTROL_H
#define MOUSE_CONTROL_H



void setup() {
    global_state = STARTUP;
    pc.baud(9600);
    
    print_battery();
    if (battery.read() < 0.7f){
        ledRed = 1;
    }
    
    
    //init_maze(); //MAZE_UPDATING should be set to false every time it saved result to mouse_action or speed_action
    global_state = WAITING;
}

void waiting() {

}

void at_beginning() {
    if (!DONE_MOVING) {
        //Wait for move to finish
    }


}

void at_center() {
    if (!DONE_MOVING) {
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
}


void speed_drive_with_distance(){
    speed_drive_cell(drive_distance);
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
                speed_turn_right(speed_drive_with_distance);
                
                break;
            case TURN_LEFT:
                speed_turn_left(speed_drive_with_distance);
                
                break;
            case TURN_RIGHT_DIAGONAL:
                speed_turn_right_diagonal(speed_drive_with_distance);
                
                break;
            case TURN_LEFT_DIAGONAL:
                speed_turn_left_diagonal(speed_drive_with_distance);
                
                break;
            case DRIVE_DIAGONAL:
                break;//TODO
        }
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
                speed_turn_right(speed_drive_with_distance);
                
                break;
            case TURN_LEFT:
                speed_turn_left(speed_drive_with_distance);
                
                break;
            case TURN_RIGHT_DIAGONAL:
                speed_turn_right_diagonal(speed_drive_with_distance);
                
                break;
            case TURN_LEFT_DIAGONAL:
                speed_turn_left_diagonal(speed_drive_with_distance);
                
                break;
            case DRIVE_DIAGONAL:
                break;//TODO
        }
    }
}







//Called by maze after computing
//Uses current direction and direction of next cell to compute the next move
void get_next_move(){
    
    switch(next_direction){
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
}




//Called when position update is called by drive
void update_position() {
    UPDATE_POSITION = false;
    //call update_maze();
}


#endif