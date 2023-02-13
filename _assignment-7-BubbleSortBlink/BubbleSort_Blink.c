// PeriodicSysTickIntsMain.c
// Use the SysTick timer to blink an LED impressively while concurrently sorting an array.
// Mark Hofmeister
// 2/11/2023

// built-in LED1 connected to P1.0


#include "msp.h"
#include <stdint.h>
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Clock.h"

volatile uint32_t Time, MainCount;      // semaphores (I think)


void main(void)
{

    Time = MainCount = 0;
    Clock_Init48MHz();              // running on crystal
    LaunchPad_Init();               // P1.0 is red LED on LaunchPad, P2.0-2.2 are RGB LED. Switches are on 1.1 and P1.4.
    EnableInterrupts();             //Enable interrupts from SysTick
    SysTick_Init(6000000,2);        // set up SysTick for 8 Hz interrupts

    uint32_t i, j, temp, length = 100;      //variables to do bubble sorting

    uint32_t a[100]={5000,5308,5614,5918,6219,6514,
    6804,7086,7361,7626,7880,8123,8354,8572,8776,8964,9137,
    9294,9434,9556,9660,9746,9813,9861,9890,9900,9890,9861,
    9813,9746,9660,9556,9434,9294,9137,8964,8776,8572,8354,
    8123,7880,7626, 7361,7086,6804,6514,6219,5918,5614,
    5308,5000,4692,4386,4082,3781,3486,3196,2914,2639,2374,
    2120,1877,1646,1428,1224,1036,863,706,566,444,340,254,
    187,139,110,100,110,139,187,254,340,444,566,706,863,
    1036, 1224, 1428, 1646,1877,2120,2374,2639,2914,
    3196,3486,3781,4082,4386,4692};

    while(1) {

        for (i = 0; i < length; i++) {
            for (j = 0; j < length - i - 1; j++) {
                if (a[j + 1] < a[j]) {

                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;

                }       //end sorting conditional
            }           //end inner for loop

            WaitForInterrupt();         //Pause sorting to wait for light blinking interrupt
            //P1->OUT ^= 0x01;
            MainCount++;

        }               //end outer for loop

    }

}

void SysTick_Handler(void){                 //handles an interrupt to blink red LED
  P2->OUT ^= 0x04;       // toggle P2.2
}

uint8_t Port1_Input(void){
  return (P1->IN&0x12);   // read P1.4,P1.1 inputs
}


