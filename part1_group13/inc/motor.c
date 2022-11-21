/*
Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// Sever VCCMD=VREG jumper on Motor Driver and Power Distribution Board and connect VCCMD to 3.3V.
//   This makes P3.7 and P3.6 low power disables for motor drivers.  0 to sleep/stop.
// Sever nSLPL=nSLPR jumper.
//   This separates P3.7 and P3.6 allowing for independent control
// Left motor direction connected to P1.7 (J2.14)
// Left motor PWM connected to P2.7/TA0CCP4 (J4.40)
// Left motor enable connected to P3.7 (J4.31)
// Right motor direction connected to P1.6 (J2.15)
// Right motor PWM connected to P2.6/TA0CCP3 (J4.39)
// Right motor enable connected to P3.6 (J2.11)

// Author:      Mohd A. Zainol
// Date:        1/10 2018
// Chip:        MSP432P401R LaunchPad Development Kit (MSP-EXP432P401R) for TI-RSLK
// File:        motor.c
// Function:    File for operating the DC motors

#include <stdint.h>
#include "msp.h"
#include "SysTick.h"

// *******Lab 12 *******

void Motor_InitSimple(void){
    // initialise P1.6 and P1.7 and make them outputs
    // Returns right away
    P1->SEL0 &= ~0xD2;
    P1->SEL1 &= ~0xD2;        // configure as GPIO
    P1->DIR |= 0x00;          // Motor Forward which is in Module 12.2
    P1->OUT |= 0xD2;          // P1.4 and P1.1 are pull-up
}

void Motor_StopSimple(uint32_t time_ms){
// Stops both motors, puts driver to sleep
// Returns right away
  P1->OUT &= ~0xC0;   // off
  P2->OUT &= ~0xC0;   // off
  SysTick_Wait10ms(time_ms); // wait for multiple of 10ms from SysTick
}

void Motor_ForwardSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;
    L = 1000-duty; // PWM using H and L
    P1->DIR |= 0xC0;
    P1->OUT &= ~0xC0;
//    P2->DIR |= ~0xC0;
    for (i=0; i<time_ms; i++){
      P2->OUT |= 0xC0;
      SysTick_Wait1us(duty);    // Stop the motor on initial state
	  P2->OUT &= ~0xC0; 
	  SysTick_Wait1us(L);    // Stop the motor on initial state
    }
}

void Motor_BackwardSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;
    L = 1000-duty; // PWM using H and L
    P1->DIR |= 0xC0;
    P2->DIR |= 0xC0;
    P1->OUT |= 0xC0;
//    P2->DIR |= 0xC0;
    for (i=0; i<time_ms; i++){
      P2->OUT |= 0xC0;
      SysTick_Wait1us(duty);    // Stop the motor on initial state
      P2->OUT &= ~0xC0;
      SysTick_Wait1us(L);    // Stop the motor on initial state
    }
}

void Motor_LeftSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;
    L = 1000-duty; // PWM using H and L
    P1->DIR |= 0xC0;
    P1->OUT |= 0x80;
    P1->OUT &= ~0x40;
//    P2->DIR |= ~0x40;
    for (i=0; i<time_ms; i++){
      P2->OUT |= 0xC0;
      SysTick_Wait1us(duty);    // Stop the motor on initial state
      P2->OUT &= ~0xC0;
      SysTick_Wait1us(L);    // Stop the motor on initial state
    }
}
void Motor_RightSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;
    L = 1000-duty; // PWM using H and L
    P1->DIR |= 0xC0;
    P1->OUT |= 0x40;
    P1->OUT &= ~0x80;
//    P2->DIR |= ~0x80;
    for (i=0; i<time_ms; i++){
      P2->OUT |= 0xC0;
      SysTick_Wait1us(duty);    // Stop the motor on initial state
      P2->OUT &= ~0xC0;
      SysTick_Wait1us(L);    // Stop the motor on initial state
    }
}
