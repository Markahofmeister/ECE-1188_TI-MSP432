// Lab11_UARTmain.c
// Runs on MSP432
// Test the functions in UART0.c by printing various
// Busy-wait device driver for the UART UCA0.
// Mark Hofmeister
// 3/16/2023

#include "msp.h"
#include <stdio.h>
#include <string.h>
//#include "../inc/Clock.h"
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

// Output RGB Value to LED
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

  //const char colors[8][10] = {"dark", "red", "blue", "green",         //array of Strings for each command
                        //"yellow", "sky blue", "white", "pink"};

  while(1)
  {
      //UART0_InString(commandPtr, maxSize);
      char test = UART0_InChar();

      if(test == 'd') {
          Port2_Output(0x00);
      }
      else if(test == 'r') {
          Port2_Output(0x01);
      }
      /*switch(*commandPtr) {

          case "dark":
              Port2_Output(0x00);
          break;
          case colors[1]:
              Port2_Output(RED);
          break;
          default:
          break;

      }*/

      Clock_Delay1ms(500);
  }

}


