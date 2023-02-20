/*
 * ECE 1188: Cyber-Physical Systems - Lab 03
 * Main file for program to periodically read sensors using software interrupts
 * Mark Hofmeister
 * 2/20/2023
 */


#include <stdint.h>
#include "msp.h"
#include "..\inc\TExaS.h"
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\SysTickInts.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\Reflectance.h"
#include "..\inc\Bump.h"

// Color    LED(s) Port2
// dark     ---    0
// red      R--    0x01
// blue     --B    0x04
// green    -G-    0x02
// yellow   RG-    0x03
// sky blue -GB    0x06
// white    RGB    0x07
// pink     R-B    0x05

#define RED       0x01
#define GREEN     0x02
#define BLUE      0x04

uint8_t sysTickCounter = 0;             // Global variable to determine how many times the SysTick Counter has been called

void Port2_Output(uint8_t data){  // write three outputs bits of P2
   P2->OUT = (P2->OUT&0xF8)|data;
}

void SysTick_Handler(void){             // Handles interrupt and determines what to do with reflection sensor

    if(sysTickCounter == 0) {           // At the beginning of each cycle, begin sensor reading

        Reflectance_Start();

    }
    else if(sysTickCounter == 1) {     //One cycle after the handler has been called, end sensor reading and reflect output in LEDs.

        uint8_t reflectanceResult = Reflectance_End();      //end RC discharge and read result

        /*uint8_t i, ledSum = 0, temp;                          //variables for for loop to determine LED output
        for (i = 0; i < 8; i++) {

            temp = (reflectanceResult >> i);                //temp variable shifts bit of interest to LSB position
            ledSum += temp & 0x01;                         //add LSB of temp to LED sum to determine output on RGB LED
        }

        Port2_Output(ledSum); */                             //output calculated value to RGB LED

        switch (reflectanceResult) {            //There must be a better way to do this.
        case 0x01:
            Port2_Output(0x00);
            break;
        case 0x02:
            Port2_Output(RED);
            break;
        case 0x45:
            Port2_Output(GREEN);
            break;
        case 0x08:
            Port2_Output(RED + GREEN);
            break;
        case 0x10:
            Port2_Output(BLUE);
            break;
        case 0x20:
            Port2_Output(RED + BLUE);
            break;
        case 0x40:
            Port2_Output(GREEN + BLUE);
            break;
        case 0x80:
            Port2_Output(RED + GREEN + BLUE);
            break;
        }

    }

    sysTickCounter++;
    if(sysTickCounter == 10) {
        sysTickCounter = 0;
    }

}

void main(void) {

    Clock_Init48MHz();              // running on crystal
    LaunchPad_Init();               // P1.0 is red LED on LaunchPad, P2.0-2.2 are RGB LED. Switches are on 1.1 and P1.4.
    SysTick_Init(48000, 2);          // Initialize SysTick to be called every 1 ms. 1e-3 / 2.08e-8 = 48000
    EnableInterrupts();             // Enable interrupts from SysTick
    P2->OUT &= ~0x07;               // Turn off LED initially

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

        // Do bubble sorting over and over again
        for (i = 0; i < length; i++) {

            for (j = 0; j < length - i - 1; j++) {
                if (a[j + 1] < a[j]) {

                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;

                }       //end sorting conditional
            }           //end inner for loop

            //WaitForInterrupt();         //Pause sorting to wait for light blinking interrupt

        }               //end outer for loop

    }


}
