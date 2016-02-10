// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Brandon Nguyen
// Date Created: 8/25/2014 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// MSB                      LSB
// PE5 PE4 PE3 PE2 PE1 PE0 B1 B0

#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "dac.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

uint32_t DACeffectsMask = 0xFFFFFFFF; //A mask to enable effects on the dac output

// **************DAC_Init*********************
// Initialize 8-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	uint8_t var;
	SYSCTL_RCGCGPIO_R |= 0x10;			//Activate PortE GPIO. PE1-4
	var++;								//Clock set delay
	var--;
	GPIO_PORTE_AMSEL_R &= ~0x3F;		//Clear xxx0 000x for no analog
	GPIO_PORTE_PCTL_R &= ~0x00FFFFFF;	//0 for PE0-5 for GPIO function
	GPIO_PORTE_DIR_R |= 0x3F;			//Set xx11 1111 for output
	GPIO_PORTE_AFSEL_R &= ~0x3F;		//Clear to disable PortE alt functions
	GPIO_PORTE_DEN_R |= 0x3F;			//Enable digital on PE0-5
  
  SYSCTL_RCGCGPIO_R |= 0x02;			//Activate PortB GPIO. PB1-0
	var++;								//Clock set delay
	var--;
	GPIO_PORTB_AMSEL_R &= ~0x03;		//Clear xxxx xx00 for no analog
	GPIO_PORTB_PCTL_R &= ~0xFFFFFF00;	//0 for PB1-0 for GPIO function
	GPIO_PORTB_DIR_R |= 0x03;			//Set xxxx xx11 for output
	GPIO_PORTB_AFSEL_R &= ~0x03;		//Clear to disable PortB alt functions
	GPIO_PORTB_DEN_R |= 0x03;			//Enable digital on PB1-0

}

// **************DAC_Out*********************
// output to DAC
// Input: 8-bit data, 0-255
// Output: none
void DAC_Out(uint32_t data){
  data &= DACeffectsMask;
  GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x3F)|((data>>2)&0x3F);
  GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & ~0x03)|((data&0x03));
}

void DAC_SetEffect(uint32_t effect){
  DACeffectsMask = effect;
}
uint32_t DAC_GetEffect(void){
  return DACeffectsMask;
}
