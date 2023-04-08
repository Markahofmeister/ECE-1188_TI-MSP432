// Lab4 - mainc
// Runs on MSP432
// Drives robot by using BLE-transmitted commands
// Mark Hofmeister
// 4/2/2023

#include "msp.h"
#include <stdio.h>
#include <string.h>
#include "Clock.h"
#include "UART0.h"
#include "Motor.h"
#include "BumpInt.h"

//Define RGB LED colors
#define RED       0x01
#define GREEN     0x02
#define BLUE      0x04

// dark --- 0
// red R-- 0x01
// blue --B 0x04
// green -G- 0x02
// yellow RG- 0x03
// sky blue -GB 0x06
// white RGB 0x07
// pink R-B 0x05

//initialize Port 2 for RGB LED
void Port2_Init(void){
  P2->SEL0 &= ~0x07;
  P2->SEL1 &= ~0x07;    // 1) configure P2.2-P2.0 as GPIO
  P2->DIR |= 0x07;      // 2) make P2.2-P2.0 out
  P2->DS |= 0x07;       // 3) activate increased drive strength
  P2->OUT &= ~0x07;     //    all LEDs off
}

// Function to output RGB Value to LED
void Port2_Output(uint8_t data){  // write three outputs bits of P2
  P2->OUT = (P2->OUT&0xF8)|data;
}

// triggered on touch, falling edge
void PORT4_IRQHandler(void){
   UART0_OutString("Collision Detected.");
   Port2_Output(RED + GREEN);
   Motor_Stop();
}



void main (void) {

    Clock_Init48MHz();              // Set system clock to 48 MHz
    UART0_Init();                   // Initialize UART on eUSCI 0
    Port2_Init();                   // Initialize RGB LED output
    Motor_Init();
    BumpInt_Init();

    char command;                   // Empty character variable to store an incoming command

    while(1) {

        command = UART0_InChar();         // Wait for incoming data transmission and store in command variable

        if(command == 'f') {         // Forwards
            Port2_Output(GREEN);
            Motor_Forward(5000,5000);

        }
        else if(command == 'b') {    // Backwards
            Port2_Output(RED);
            Motor_Backward(5000,5000);
        }
        else if(command == 's') {    // Stop
            Port2_Output(BLUE);
            Motor_Stop();
        }

        //Clock_Delay1ms(500);              //Delay for 500 ms

    }



}
