#ifndef MOUSE_CONTROL_H
#define MOUSE_CONTROL_H


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

void setup() {
    global_state = STARTUP;
    pc.baud(9600);
    
    print_battery();
    if (battery.read() < 0.70f){
        ledRed = 1;
    }
    
    
    //init_maze(); //MAZE_UPDATING should be set to false every time it saved result to mouse_action or speed_action
    global_state = WAITING;
}





//Called when position update is called by drive
void update_position() {
    UPDATE_POSITION = false;
    UPDATE_FINISHED = false;
    //Update to next cell first
    
    //Set walls
    //then call update_maze();
    
    UPDATE_FINISHED = true;
}

//Updates ALL distance values in maze, so will be accurate on next run
void full_update_maze(){

}

void save_valid_maze(){
    full_update_maze();
    //then saves    
}

//Restores assuming at beginnning
void restore_valid_maze(){
    
}





void waiting() {
    //Does nothing while waiting for something else async to finish
}

void lost(){
    
    //Wait for user to reset. Then restore_valid_maze
    //Mouse should always be waiting for the user to reset, in which case restores valid maze every time
    
    
}



void arrived_at_beginning() {
    global_state = WAITING;
    
    while (!DONE_MOVING) {
        //Wait for move to finish
    }
    
    //Verify that accurately arrived at beginning. If not, lost
    if (current_direction == SOUTH && leftIR.readIR() < 12 && rightIR.readIR() < 12 && rightFrontIR.readIR() < 12){
        save_valid_maze();
        global_state = AT_BEGINNING;
    }
    else {
        global_state = LOST;
        
    }
    


}

bool verify_at_center(){
    
    
    return false;
}

void arrived_at_center() {
    global_state = WAITING;
    
    while (!DONE_MOVING) {
        //Wait for move to finish
    }
    
    

    //Drive all 4 center cells to verify that is at center
    if (verify_at_center()){
        save_valid_maze();                   
        global_state = AT_CENTER;
    }
    else {
        global_state = LOST;
    }

}

void at_beginning(){
    
}

void at_center(){
    
}

void exploring_to_center() {
    if (UPDATE_POSITION) {
        update_position();
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
}

void exploring_back() {
    if (UPDATE_POSITION) {
        update_position();
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
}


void speed_drive_with_distance(){
    speed_drive_cell(drive_distance);
}

void speeding_to_center() {
    if (UPDATE_POSITION) {
        update_position();
    }

    if (UPDATING_INSTRUCTIONS) {
        //Wait for instructions to finish
    }
    else if (DONE_MOVING) {
        UPDATE_FINISHED = false;
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
    if (UPDATE_POSITION) {
        update_position();
    }
    
    if (UPDATING_INSTRUCTIONS) {
        //Wait for instructions to finish
    }
    else if (DONE_MOVING) {
        UPDATE_FINISHED = false;
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
}






#endif