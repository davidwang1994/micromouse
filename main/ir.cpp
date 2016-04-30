#include "mbed.h"
#include "ir.h"


/*
 * front_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from FRONT WALL
 */
float front_wall_dist()
{
    float right = rightFrontIR;
    
    return rightFrontIR;
}

/*
 * left_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from LEFT WALL
 */
float left_wall_dist()
{
    return leftIR;
}

/*
 * right_wall_dist
 * Parameters: None
 * Returns: float, distance in cm from RIGHT WALL
 */
float right_wall_dist()
{
    return rightIR;
}


////has_left_wall, for PID control
//bool has_left_wall()
//{
//    return leftIR < 12;
//}
//
////has_right_wall, for PID control
//bool has_right_wall()
//{
//    return rightIR < 12;
//}
//
bool has_front_wall() {
    return rightFrontIR < 12;
}