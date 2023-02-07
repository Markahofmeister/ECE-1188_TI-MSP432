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


void Port3_Init(void){

  P3->SEL0 &= ~0x0C;
  P3->SEL1 &= ~0x0C;   // Configure P3.2  P3.3 as GPIO
  P3->DIR &= ~0x0C;    // Make P3.2 and P3.3 in
  P3->REN &= ~0x0C;    // Disable pull resistors on P3.2 and P3.3

}

void Port6_Init(void){

  P6->SEL0 &= ~0x0C;
  P6->SEL1 &= ~0x0C;   // Configure P6.2  P6.3 as GPIO
  P6->DIR |= 0x0C;    // Make P6.2 and P6.3 outputs
  P6->REN &= ~0x0C;    // Disable pull resistors on P3.2 and P3.3

  P6->OUT &= ~0x0C;     //    all output pins off
}

uint8_t Port3_Input(void){
  return (P3->IN&0x0C);   // read P3.2,P3.3 inputs
}

void Port6_Output(uint8_t data){  // write output bits of P6
  P6->OUT = (P2->OUT&0xF3)|data;
}

int main(void){ uint8_t status;

  Port3_Init();                    // initialize P1.1 and P1.4 and make them inputs (P1.1 and P1.4 built-in buttons)

  Port6_Init();                    // initialize P2.2-P2.0 and make them outputs (P2.2-P2.0 built-in LEDs)

  while(1){

    status = Port3_Input();         //Retrieve switch status from port 1 input (SW1 and SW2)

    switch(status){                 // switches are negative logic on P1.1 and P1.4

      case 0x08:                    // SW0 pressed
        Port6_Output(0x08);
        break;
      case 0x04:                    // SW0 pressed
        Port6_Output(0x04);
        break;
      case 0x00:                    // both switches pressed
        Port6_Output(0x0C);
        break;
      case 0x0C:                    // neither switch pressed
        Port6_Output(0x00);
        break;

    }

  }

}
