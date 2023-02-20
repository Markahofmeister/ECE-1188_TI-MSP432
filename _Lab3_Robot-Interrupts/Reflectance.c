/*
 * ECE 1188: Cyber-Physical Systems - Lab 03
 * Code to operate reflectance sensors
 * Mark Hofmeister
 * 2/20/2023
 */

// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2

// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"

// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
// Input: none
// Output: none

void Reflectance_Init(void) {               //From Lab 2

    //Initialize CTRL EVEN to output on P5.3
    // 0000 1000 = 0x08
    uint8_t ceMask = 0x08;
    P5->SEL0 &= ~ceMask;                  // set P5.3 to GPIO
    P5->SEL1 &= ~ceMask;
    P5->DIR  |=  ceMask;                  // set P5.3 to output
    P5->OUT  &= ~ceMask;                  // P5.3 should be off at first
    P5->REN  &= ~ceMask;                  // turn off PUR?

    //Initialize CTRL ODD to output on P9.2
    // 0000 0100 = 0x04
    uint8_t coMask = 0x04;
    P9->SEL0 &= ~coMask;                  // set P9.2 to GPIO
    P9->SEL1 &= ~coMask;
    P9->DIR  |=  coMask;                  // set P9.2 to output
    P9->OUT  &= ~coMask;                  // P9.2 should be off at first

    //Initialize bits 0-7 of QTRX module as inputs (initially) on P7.0-7.7
    // 1111 1111 = 0xFF
    uint8_t qtrxMask = 0xFF;
    P7->SEL0 = ~qtrxMask;                  // set P7.0-7.7 as GPIO
    P7->SEL1 = ~qtrxMask;
    P7->DIR  = ~qtrxMask;                 // set P7.0-7.7 as input

}


// ------------Reflectance_Start------------
// Begin reading the eight sensors, but don't end so that we can do other code things before reading return value
// turn on LEDs
// Make LEDs output
// Charge capacitor for measurement
// make LEDs inputs
// Input: none
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
void Reflectance_Start(void){

    uint8_t ceMask = 0x08;          //mask for even LED control
    uint8_t coMask = 0x04;          //mask for odd LED control
    uint8_t qtrxMask = 0xFF;        //mask for control of QTRX LEDs

    P5->OUT |= ceMask;              // turn on 4 even IR LEDs
    P9->OUT |= coMask;              // turn on 4 odd IR LEDs
    P7->DIR = qtrxMask;             // make P7.7-P7.0 out
    P7->OUT = qtrxMask;             // prime capacitor for measurement

    Clock_Delay1us(10);             // wait 10 us for cap to charge
    P7->DIR = ~qtrxMask;            // make P7.7-P7.0 in

}


// ------------Reflectance_End------------
// Finish reading the eight sensors
// Read sensors
// Turn off the 8 IR LEDs
// Input: none
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
// Assumes: Reflectance_Start() was called 1 ms ago
uint8_t Reflectance_End(void){

    uint8_t result,qtrxMask = 0xFF;  //mask for control of QTRX LEDs, result to return
    result = P7->IN;                 // read in result from IR LEDs

    P7->OUT = ~qtrxMask;             // turn off all 7 IR LEDs

    return result;

}







