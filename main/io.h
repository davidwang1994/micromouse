#ifndef IO_H
#define IO_H

#include "drive_control.h"
volatile float speed_adjust = 0;
volatile float _errorP = 0;
volatile float _irr = 0;
volatile float _irl = 0;

void print_battery(){
    pc.printf("BATTERY: %f\r\n", battery.read());
}

void print_motor(){
    pc.printf("lm %f\r\n" , leftMotor.read());
    pc.printf("rm %f\r\n" , rightMotor.read());
    
    //pc.printf("irl %f\r\n" , _irl);
//    pc.printf("irr %f\r\n" , _irr);
//    pc.printf("ep %f\r\n" , _errorP);
//    pc.printf("sa %f\r\n" , speed_adjust);
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
    pc.printf("\r\nlir %f\r\n", leftIR.readIR());
    //print_ir_raw(leftIR);
    //pc.printf("LeftAvg: %f\r\n", leftIR.value);
    
    pc.printf("rir %f\r\n", rightIR.readIR());
    //print_ir_raw(rightIR);
    //pc.printf("Right_Avg: %f\r\n", rightIR.value);
    
    //pc.printf("Front: %f\r\n", rightFrontIR.read());
    //print_ir_raw(rightFrontIR);
    //pc.printf("Front_Avg: %f\r\n", rightFrontIR.value);
    //pc.printf("Motor Speed: %.2f\r\n", speed);
}



void print_status(){
    //print_encoder();
    print_ir();
    print_motor();
    
    
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