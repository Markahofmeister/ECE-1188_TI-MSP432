// Lab12_Motorsmain.c
// Runs on MSP432
// Mark Hofmeister
// 2/2/2023

//**************RSLK1.1***************************
// Left motor direction connected to P5.4 (J3.29)
// Left motor PWM connected to P2.7/TA0CCP4 (J4.40)
// Left motor enable connected to P3.7 (J4.31)
// Right motor direction connected to P5.5 (J3.30)
// Right motor PWM connected to P2.6/TA0CCP3 (J4.39)
// Right motor enable connected to P3.6 (J2.11)

#include "msp.h"
#include "..\inc\bump.h"
#include "..\inc\Clock.h"
#include "..\inc\SysTick.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\MotorSimple.h"

// Driver test
void Pause(void){

  while(LaunchPad_Input()==0);  // wait for touch
  while(LaunchPad_Input());     // wait for release

}

int myTestProg(void) {

    uint16_t fastDuty, slowDuty, timeLinSlow, timeLinFast, timeTurn;
      timeLinSlow = 100;
      timeLinFast= 30;
      timeTurn = 75;
      fastDuty = 7500; slowDuty = 2500;

      while(1){

        Pause();

        Motor_ForwardSimple(slowDuty, timeLinSlow);     //Ahead slow

        Motor_BackwardSimple(slowDuty, timeLinSlow);    //Backwards slow

        Motor_ForwardSimple(fastDuty, timeLinFast);     //Ahead fast

        Motor_BackwardSimple(fastDuty, timeLinFast);    //Backwards fast

        Motor_LeftSimple(slowDuty, timeTurn);           //Left Slow

        Motor_RightSimple(slowDuty, timeTurn*2);        //Right Slow

        Motor_LeftSimple(slowDuty, timeTurn);           //Left slow (back to center)

        Motor_LeftSimple(fastDuty, timeTurn);           //Left fast

        Motor_RightSimple(fastDuty, timeTurn*2);        //Right fast

        Motor_LeftSimple(fastDuty, timeTurn);           //Left fast (back to center)

      }

}


int main(void){


  Clock_Init48MHz();
  SysTick_Init();
  LaunchPad_Init();   // built-in switches and LEDs
  Motor_InitSimple(); // initialization

  while(1) {

     myTestProg();

  }

}



