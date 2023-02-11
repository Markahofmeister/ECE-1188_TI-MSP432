// FSM_Controller.c
// Mark Hofmeister
// February 6th, 2023

#include <stdint.h>
#include "msp.h"
#include "..\inc\TExaS.h"
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\SysTick.h"
#include "..\inc\LaunchPad.h"

#define DIME      0x02                  // on the left side of the LaunchPad board
#define NICKEL    0x10                  // on the right side of the LaunchPad board
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

// Define struct of states with the output value, time, and pointer to 4 more states
struct State {

  uint8_t Out;             // 6-bit output
  uint32_t Time;           // 1 ms
  const struct State *Next[4];      // depends on 2-bit input

};

typedef const struct State State_t;     //typedef to make invoking struct easier

//Define pointers to each state of the FSM
#define S1   &FSM[0]
#define S2   &FSM[1]
#define S3   &FSM[2]
#define S4   &FSM[3]
#define S5   &FSM[4]

#define delay_ms 1000

/*
//define FSM of 5 states - RGB LED
State_t FSM[5] = {
 {BLUE, delay_ms, {S1, S3, S2, S1} },              //State 1 - 0 cents - blue LED
 {GREEN, delay_ms,{S2, S4, S3, S2} },              //State 2 - 5 cents - green LED
 {RED+GREEN, delay_ms,{S3, S5, S4, S3} },          //State 3 - 10 cents - yellow LED
 {GREEN+BLUE, delay_ms,{S1, S1, S1, S1} },         //State 4 - 15 cents - sky blue
 {RED, delay_ms, {S2, S2, S2, S2} }                //State 5 - 20 cents - red (on both RGB LED and P1) LED
};*/

//define FSM of 5 states - Red LED
State_t FSM[5] = {
 {0x00, delay_ms, {S1, S3, S2, S1} },               //State 1 - 0 cents - no LED
 {0x00, delay_ms,{S2, S4, S3, S2} },                //State 2 - 5 cents - no LED
 {0x00, delay_ms,{S3, S5, S4, S3} },                //State 3 - 10 cents - no LED
 {0x01, delay_ms,{S1, S1, S1, S1} },                //State 4 - 15 cents - Red LED
 {0x01, delay_ms, {S2, S2, S2, S2} }                //State 5 - 20 cents - Red LED
};


//Initialize port 1 for both reading buttons and for writing Red LED
void Port1_Init(void){

  //Buttons (Input)
      P1->SEL0 &= ~0x12;
      P1->SEL1 &= ~0x12;   // 1) configure P1.4  P1.1 P1.0 as GPIO
      P1->DIR &= ~0x12;    // 2) make P1.4 and P1.1 in
      P1->REN |= 0x12;     // 3) enable pull resistors on P1.4 and P1.1
      P1->OUT |= 0x12;     //    P1.4 and P1.1 are pull-up

  //LED (Output)
      P1->SEL0 &= ~0x01;   //Configure P1.0 LED as GPIO
      P2->SEL1 &= ~0x01;
      P1->DIR |= 0x01;     //P1.0 is an output
      P1->REN |= 0x01;     //Enable pull resistor
      P1->OUT &= ~0x01;    //Set value low

}
uint8_t Port1_Input(void){
  return (P1->IN&0x12);   // read P1.4,P1.1 inputs
}

void Port1_Output(uint8_t data) {
    P1->OUT = (P1->OUT&0xFE)|data;
}

//initialize Port 2 for RGB LED
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

void main(void)
{

    State_t *statePtr;              // state pointer
    uint8_t input;
    Clock_Init48MHz();              // initialize clock to 48MHz

    Port1_Init();                   //initialize ports for peripherals
    Port2_Init();

    statePtr = S1;                       //initialize to default state 1

    while(1) {

        //Port2_Output(statePtr->Out);     //write output of current state
        Port1_Output(statePtr->Out);
        Clock_Delay1ms(statePtr->Time);  // wait 1 ms * current state's Time value

        input = (Port1_Input() & 0x12);        //read switch inputs

        uint8_t tempStr1 = input >> 1;          //Shift the contents of bits 1 and 4 into bits 0 and 1 (respectively)
        uint8_t tempStr2 = input >> 3;
        input = tempStr1;
        input |= tempStr2;
        input &= 0x03;


        statePtr = statePtr->Next[input];      // transition to next state

    }

}

