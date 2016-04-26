#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define NORTH_WEST 4
#define NORTH_EAST 5
#define SOUTH_WEST 6
#define SOUTH_EAST 7


//Internal for elsewhere
int direction = NORTH; //maze internal

//Signals from/to elsewhere:
int current_direction = NORTH;
int next_direction = -1;
int drive_distance = 0; //In number of cells, including diagonal across the cell (counts as 1 cell)

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

#include "drive_control.h"
#include "io.h"
#include "maze.h"
#include "mouse_control.h"

Serial pc(PA_9, PA_10);  //THIS MUST GO IN MAIN






Ticker ticker;
int main() {

    setup();
    
    wait(0.3);
    print_ir();
    ticker.attach(&print_encoder, 1);
    
    wait(2);
    drive_cell();
    
    
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
        
        if (UPDATE_POSITION) {
            update_position();
        }
    }
}
