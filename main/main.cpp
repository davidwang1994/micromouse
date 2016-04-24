#include "drive_control.h"
Serial pc(PA_9, PA_10);  //THIS MUST GO IN MAIN



Ticker ticker;

float speed;
void printStatus(){
    pc.printf("\r\nLeft: %f\r\n", leftIR.read());
    pc.printf("Left: %f\r\n", leftIR.value);
    pc.printf("Right: %f\r\n", rightIR.read());
    pc.printf("Right: %f\r\n", rightIR.value);
    pc.printf("Front: %f\r\n", rightFrontIR.read());
    pc.printf("Front: %f\r\n", rightFrontIR.value);
    //pc.printf("Motor Speed: %.2f\r\n", speed);
}


DigitalOut led(LED1);

extern "C" void mbed_reset();

int main() {
  pc.baud(9600);
  
  //pc.printf("\r\nHi!\r\n");
  
  speed = 0.0f;
  
  printStatus();
  
  //ticker.attach(&printStatus, 1);
  
  //Imperial march on buzzer
  float W = 3; //whole 4/4
  float H = W/2; //half 2/4
  float Q = H/2; //quarter 1/4
  float E = Q/2; //eighth 1/8
  float S = E/2; // sixteenth 1/16

    wait(1);


  while(1){
    
    
    /*turn(0.05);
    wait(1);
    stop();
    wait(1);
    turn(-0.05);
    wait(1);
    stop();*/
    wait(1);
    
    /*buzzer.play(_LA3,Q);
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
    buzzer.play(_OFF,W);*/
  }
}
