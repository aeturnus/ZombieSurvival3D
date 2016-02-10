// dac.h
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Brandon Nguyen
// Date Created: 8/25/2014 
// Last Modified: 10/5/2014 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// MSB                      LSB
// PE5 PE4 PE3 PE2 PE1 PE0 B1 B0
#ifndef __DAC_H__
#define __DAC_H__

#include <stdint.h>

// Header files contain the prototypes for public functions
// this file explains what the module does

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 8-bit data, 0 to 255 
// Output: none
void DAC_Out(uint32_t data);

void DAC_SetEffect(uint32_t effect);
uint32_t DAC_GetEffect(void);

#endif
