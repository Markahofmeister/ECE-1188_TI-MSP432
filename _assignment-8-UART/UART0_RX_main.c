// Lab11_UARTmain.c
// Runs on MSP432
// Test the functions in UART0.c by printing various
// Busy-wait device driver for the UART UCA0.
// Mark Hofmeister
// 3/16/2023

#include "msp.h"
#include <stdio.h>
#include <string.h>
#include "Clock.h"
#include "../inc/UART0.h"

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

void main(void)
{

  Clock_Init48MHz();                    // Set system clock to 48 MHz
  UART0_Init();
  Port2_Init();

  uint16_t maxSize = 9;     // Max command length that we'll see

  char command[10];                      // Empty buffer in which UART characters can be sent
  char *commandPtr = command;             // Pointer to buffer (to pass to function)

  while(1)
  {
      UART0_InString(commandPtr, maxSize);      // Store incoming String from UART in pointer to array of characters

      if(!strcmp(commandPtr, "dark")) {         // Illuminate RGB LED color based upon the contents of the UART-sent string
          Port2_Output(0x00);
      }
      else if(!strcmp(commandPtr, "red")) {
          Port2_Output(RED);
      }
      else if(!strcmp(commandPtr, "blue")) {
          Port2_Output(BLUE);
      }
      else if(!strcmp(commandPtr, "green")) {
          Port2_Output(GREEN);
      }
      else if(!strcmp(commandPtr, "yellow")) {
          Port2_Output(RED + GREEN);
      }
      else if(!strcmp(commandPtr, "sky blue")) {
          Port2_Output(GREEN + BLUE);
      }
      else if(!strcmp(commandPtr, "white")) {
          Port2_Output(RED + GREEN + BLUE);
      }
      else if(!strcmp(commandPtr, "pink")) {
          Port2_Output(RED + BLUE);
      }

      Clock_Delay1ms(500);                  // Delay 500 ms
  }

}


