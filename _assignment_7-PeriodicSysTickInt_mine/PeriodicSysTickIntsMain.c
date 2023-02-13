// PeriodicSysTickIntsMain.c
// Use the SysTick timer to request interrupts at a particular period.
// Mark Hofmeister
// 2/11/2023

// built-in LED1 connected to P1.0
// P1.0, P2.0 are an output to profiling scope/logic analyzer

#include <stdint.h>
#include "msp.h"
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Clock.h"

volatile uint32_t Time, MainCount;

#define LEDOUT (*((volatile uint8_t *)(0x42098040)))        //what is this?


void SysTick_Handler(void){
  LEDOUT ^= 0x01;       // toggle P1.0
  LEDOUT ^= 0x01;       // toggle P1.0
  Time = Time + 1;
  LEDOUT ^= 0x01;       // toggle P1.0
}


int main(void){
  Clock_Init48MHz();      // running on crystal
  Time = MainCount = 0;
  SysTick_Init(6000000,2);  // set up SysTick for 8 Hz interrupts
  LaunchPad_Init();       // P1.0 is red LED on LaunchPad
  EnableInterrupts();
  while(1){
    WaitForInterrupt();
    P2->OUT ^= 0x01; // foreground thread
    MainCount++;
  }
}

/*
uint32_t High=120000,Low=360000;
void SysTick_Handler2(void){ // PWM
  LEDOUT ^= 0x01;   // toggle red LED
  if(LEDOUT){  // if high
     SysTick->LOAD = High; // time while high
  }else{
     SysTick->LOAD = Low;  // time while low
  }
  SysTick->VAL = 0;        // any value written to counter clears
  Time = Time + 1;
}*/
