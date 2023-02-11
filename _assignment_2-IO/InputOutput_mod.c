// main-1.c
// Runs on MSP432
// Only SW1 makes LED yellow
// Only SW2 makes LED light blue
// Both SW1 and SW2 makes LED white
// Neither SW1 or SW2 makes LED green
// Neither SW1 or SW2 turns LEDs off

// Mark Hofmeister
// 1/22/2023

// negative logic built-in Button 1 connected to P1.1
// negative logic built-in Button 2 connected to P1.4
// built-in red LED connected to P2.0
// built-in green LED connected to P2.1
// built-in blue LED connected to P2.2
// Color    LED(s) Port2
// dark     ---    0
// red      R--    0x01
// blue     --B    0x04
// green    -G-    0x02
// yellow   RG-    0x03
// sky blue -GB    0x06
// white    RGB    0x07
// pink     R-B    0x05

#include <stdint.h>
#include "msp.h"

#define SW1       0x02                  // on the left side of the LaunchPad board
#define SW2       0x10                  // on the right side of the LaunchPad board
#define RED       0x01
#define GREEN     0x02
#define BLUE      0x04



void Port1_Init(void){
  P1->SEL0 &= ~0x12;
  P1->SEL1 &= ~0x12;   // 1) configure P1.4  P1.1 P1.0 as GPIO
  P1->DIR &= ~0x12;    // 2) make P1.4 and P1.1 in
  P1->REN |= 0x12;     // 3) enable pull resistors on P1.4 and P1.1
  P1->OUT |= 0x12;     //    P1.4 and P1.1 are pull-up
}
uint8_t Port1_Input(void){
  return (P1->IN&0x12);   // read P1.4,P1.1 inputs
}

void Port2_Init(void){
  P2->SEL0 &= ~0x07;
  P2->SEL1 &= ~0x07;    // 1) configure P2.2-P2.0 as GPIO
  P2->DIR |= 0x07;      // 2) make P2.2-P2.0 out
  P2->DS |= 0x07;       // 3) activate increased drive strength
  P2->OUT &= ~0x07;     //    all LEDs off
}
void Port2_Output(uint8_t data){  // write three outputs bits of P2
  P2->OUT = (P2->OUT&0xF8)|data;
}
int main(void){ uint8_t status;
  Port1_Init();                    // initialize P1.1 and P1.4 and make them inputs (P1.1 and P1.4 built-in buttons)

  Port2_Init();                    // initialize P2.2-P2.0 and make them outputs (P2.2-P2.0 built-in LEDs)

  while(1){

    status = Port1_Input();         //Retrieve switch status from port 1 input (SW1 and SW2)

    switch(status){                 // switches are negative logic on P1.1 and P1.4

      case 0x10:                    // SW1 pressed
        Port2_Output(RED+GREEN);
        break;
      case 0x02:                    // SW2 pressed
        Port2_Output(GREEN+BLUE);
        break;
      case 0x00:                    // both switches pressed
        Port2_Output(RED+GREEN+BLUE);
        break;
      case 0x12:                    // neither switch pressed
        Port2_Output(GREEN);
        break;

    }

  }

}
