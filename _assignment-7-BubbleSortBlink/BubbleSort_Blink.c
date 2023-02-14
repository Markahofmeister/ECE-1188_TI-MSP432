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

//volatile uint32_t Time, MainCount;      // semaphores (I think)
volatile uint8_t switchInput;             // semaphore to determine what action the interrupt handler should take

uint8_t Port1_Input(void){
  return (P1->IN&0x12);                 // read P1.4,P1.1 inputs
}

void SysTick_Handler(void){                 //handles interrupt to blink one of the LEDs

    if(switchInput == 0x10) {               //If SW1 has been depressed, blink red LED (will be 8 Hz)
        P1->OUT ^= 0x01;
    }
    else if(switchInput == 0x02) {          //If SW2 has been depressed, blink blue LED (will be 64 Hz)
        P2->OUT ^= 0x04;
    }
    else {                                  //If no input, turn off all LEDs
        P1->OUT &= ~0x01;
        P2->OUT &= ~0x04;
    }
}


void main(void)
{
    Clock_Init48MHz();              // running on crystal
    LaunchPad_Init();               // P1.0 is red LED on LaunchPad, P2.0-2.2 are RGB LED. Switches are on 1.1 and P1.4.
    SysTick_Init(6000000,2);
    EnableInterrupts();             //Enable interrupts from SysTick
    P1->OUT &= ~0x01;               //Turn off LED initially

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

            //Read switch values using conditional
            //reload SysTick value based on conditional
            //proceed to inner for loop

            //uint8_t switchInput = Port1_Input();
            switchInput = Port1_Input();

            if(switchInput == 0x02) {       //if switch 1 has been pressed

                SysTick_Init(93750,2);        // set up SysTick for 64 Hz interrupts

            }
            else if(switchInput == 0x10) {  //if switch 2 has been pressed

                SysTick_Init(6000000,2);          //set up SysTick for 8 Hz interrupts

            }
            // (Switches seem to be reversed on my board. IDK why.)

            for (j = 0; j < length - i - 1; j++) {
                if (a[j + 1] < a[j]) {

                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;

                }       //end sorting conditional
            }           //end inner for loop

            WaitForInterrupt();         //Pause sorting to wait for light blinking interrupt
            //MainCount++;

        }               //end outer for loop

    }


}

