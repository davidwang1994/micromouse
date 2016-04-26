#ifndef IO_H
#define IO_H

#include "drive_control.h"

void print_battery(){
    pc.printf(" %f\r\n", battery.read());
}

void print_encoder(){
    pc.printf("left encoder: %d\r\n" , leftEncoder);
    pc.printf("right encoder: %d\r\n" , rightEncoder);
}

void print_ir_raw (IRSensor ir){
    for (int i = 0; i < 5; i ++){
        pc.printf(" %f\r\n", ir.last_read[i]);
    }
}

void print_ir(){
    pc.printf("\r\nLeft: %f\r\n", leftIR.read());
    print_ir_raw(leftIR);
    pc.printf("LeftAvg: %f\r\n", leftIR.value);
    
    pc.printf("Right: %f\r\n", rightIR.read());
    print_ir_raw(rightIR);
    pc.printf("Right_Avg: %f\r\n", rightIR.value);
    
    pc.printf("Front: %f\r\n", rightFrontIR.read());
    print_ir_raw(rightFrontIR);
    pc.printf("Front_Avg: %f\r\n", rightFrontIR.value);
    //pc.printf("Motor Speed: %.2f\r\n", speed);
}


void play_imperial_march(){
    float W = 3; //whole 4/4
    float H = W/2; //half 2/4
    float Q = H/2; //quarter 1/4
    float E = Q/2; //eighth 1/8
    float S = E/2; // sixteenth 1/16
    buzzer.play(_LA3,Q);
    buzzer.play(_LA3,Q);
    buzzer.play(_LA3,Q);
    buzzer.play(_F3,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_LA3,Q);
    buzzer.play(_F3,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_LA3,H);
    buzzer.play(_E4,Q);
    buzzer.play(_E4,Q);
    buzzer.play(_E4,Q);
    buzzer.play(_F4,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_Ab3,Q);
    buzzer.play(_F3,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_LA3,H);
    buzzer.play(_LA4,Q);
    buzzer.play(_LA3,E+S);
    buzzer.play(_LA3,S);
    buzzer.play(_LA4,Q);
    buzzer.play(_Ab4,E+S);
    buzzer.play(_G4,S);
    buzzer.play(_Gb4,S);
    buzzer.play(_E4,S);
    buzzer.play(_F4,E);
    buzzer.play(_OFF,E);
    buzzer.play(_Bb3,E);
    buzzer.play(_Eb4,Q);
    buzzer.play(_D4,E+S);
    buzzer.play(_Db4,S);
    buzzer.play(_C4,S);
    buzzer.play(_B3,S);
    buzzer.play(_C4,E);
    buzzer.play(_OFF,E);
    buzzer.play(_F3,E);
    buzzer.play(_Ab3,Q);
    buzzer.play(_F3,E+S);
    buzzer.play(_LA3,S);
    buzzer.play(_C4,Q);
    buzzer.play(_LA3,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_E4,H);
    buzzer.play(_LA4,Q);
    buzzer.play(_LA3,E+S);
    buzzer.play(_LA3,S);
    buzzer.play(_LA4,Q);
    buzzer.play(_Ab4,E+S);
    buzzer.play(_G4,S);
    buzzer.play(_Gb4,S);
    buzzer.play(_E4,S);
    buzzer.play(_F4,E);
    buzzer.play(_OFF,E);
    buzzer.play(_Bb3,E);
    buzzer.play(_Eb4,Q);
    buzzer.play(_D4,E+S);
    buzzer.play(_Db4,S);
    buzzer.play(_C4,S);
    buzzer.play(_B3,S);
    buzzer.play(_C4,E);
    buzzer.play(_OFF,E);
    buzzer.play(_F3,E);
    buzzer.play(_Ab3,Q);
    buzzer.play(_F3,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_LA3,Q);
    buzzer.play(_F3,E+S);
    buzzer.play(_C4,S);
    buzzer.play(_LA3,H);
    buzzer.play(_OFF,W);
}


#endif