// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: Brandon Nguyen
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"
void Ain1_Init(void)
{
	//Pinout stuff
	SYSCTL_RCGCGPIO_R |= 0x10;			// Enable clock for port e
	while(SYSCTL_RCGCGPIO_R&0x10 == 0){}
  __asm
  {
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
  }
	GPIO_PORTE_DIR_R &= ~0x04;			// PE2 is input
	GPIO_PORTE_AFSEL_R |= 0x04;			// PE2 is alternate func
	GPIO_PORTE_DEN_R &= ~0x04;			// No digital for PE2
	GPIO_PORTE_AMSEL_R |= 0x04;		// Enable analog for PE2
	
	//ADC stuff
	SYSCTL_RCGCADC_R |= 0x1;				// Set bit 0 of clock
	while(SYSCTL_RCGCADC_R &0x1 == 0){}
    __asm
  {
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
  }
	ADC0_PC_R = 0x01;								// Set 125kHz conversion speed
	ADC0_SSPRI_R = 0x0123;								// Set priority 0
	ADC0_ACTSS_R &= ~0x08;					// Clear bit 3
																	//     15    11    7     3
	ADC0_EMUX_R	&= ~0xF000;					// xxxx xxxxx xxxx xxxx xxxx Software start trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&~0xF) | (0x1);	// clear bits 3-0 and put the channel number (PE2 is 1)
	ADC0_SSCTL3_R = (0x6);// Set sample control bits: disable temp, notify on sample ocmplete, indicate single sampl, denote single ended signal mode
	ADC0_IM_R	&= ~0x8;							// Disable interrupts
	ADC0_ACTSS_R |= 0x8;						// Enable sequencer 3
}

void Ain9_Init(void)
{
	//Pinout stuff
	SYSCTL_RCGCGPIO_R |= 0x10;			// Enable clock for port e
	while(SYSCTL_RCGCGPIO_R&0x10 == 0){}
  __asm
  {
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
  }
	GPIO_PORTE_DIR_R &= ~0x10;			// PE4 is input
	GPIO_PORTE_AFSEL_R |= 0x10;			// PE4 is alternate func
	GPIO_PORTE_DEN_R &= ~0x10;			// No digital for PE4
	GPIO_PORTE_AMSEL_R |= 0x10;		// Enable analog for PE4
	
	//ADC stuff
	SYSCTL_RCGCADC_R |= 0x1;				// Set bit 0 of clock
	while(SYSCTL_RCGCADC_R &0x1 == 0){}
  __asm
  {
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
  }
	ADC0_PC_R = 0x01;								// Set 125kHz conversion speed
	ADC0_SSPRI_R = 0x0123;								// Set priority 0
	ADC0_ACTSS_R &= ~0x03;					// Disable sequence 2
																	//     15    11    7     3
	ADC0_EMUX_R	&= ~0x0F00;					// xxxx xxxxx xxxx xxxx xxxx Software start trigger
	ADC0_SSMUX2_R = (ADC0_SSMUX2_R&~0xF) | (0x9);	// clear bits 3-0 and put the channel number (PE2 is 1)
	ADC0_SSCTL2_R = (0x6);// Set sample control bits: disable temp, notify on sample ocmplete, indicate single sampl, denote single ended signal mode
	ADC0_IM_R	&= ~0x4;							// Disable interrupts
	ADC0_ACTSS_R |= 0x4;						// Enable sequencer 2
}

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	//Pinout stuff
	Ain1_Init();
	Ain9_Init();
}



//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: Pointer to buffer
// Output: none
void ADC_In(uint32_t* data)
{ 	
	ADC0_PSSI_R = 0x000C;
	while((ADC0_RIS_R&0x08)==0){};
	data[0] = ADC0_SSFIFO3_R&0xFFF; //Ain9
	while((ADC0_RIS_R&0x04)==0){};
	data[1] = ADC0_SSFIFO2_R&0xFFF; //Ain1
	ADC0_ISC_R = 0x000C;
}