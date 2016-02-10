// Lab8.c
// Runs on LM4F120 or TM4C123
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 3/6/2015 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats

#include <stdint.h>

#include "ST7735.h"
#include "../TExaS.h"
#include "print.h"
#include "../../tm4c123gh6pm.h"
#include "../../brandonware/BrandonTypes.h"
#include "../../brandonware/BrandonMath.h"
#include "../../brandonware/BrandonBufferManager.h"
#include "../../brandonware/BrandonRaycaster.h"
#include "resource.h"
//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF_CLEAR(bit) GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R &(~(0x01<<bit))
#define PF_SET(bit) GPIO_PORTF_DATA_R |= (0x01<<bit)
#define PF_EOR(bit) GPIO_PORTF_DATA_R ^= (0x01<<bit)
#define PF_GET(bit) ((GPIO_PORTF_DATA_R & (0x01<<bit))>>bit)




#define COLUMNS 160
#define ROWS 90



void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;													//Disable SysTick during init
	NVIC_ST_RELOAD_R = 2000000;							//Reload value
	NVIC_ST_CURRENT_R = 0;											//Clear current
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;		//Priority 1
	NVIC_ST_CTRL_R = 0x0007;										//Enable SysTick w/ core clock and int
}

uint8_t doneDrawing = 0;  //here's a flag to skip a frame. set it when you're done drawing to the buffer. This'll tell if you if you should send the buffer to the lcd
void SysTick_Handler(void)
{
  //Feel free to use it to keep a constant framerate. TBH i'd use a timer. I'll update this later perhaps
}


// Initialize Port F so PF1, PF2 and PF3 are heartbeats,PF0 and PF4 are inputs
void PortF_Init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x20; //Enable clock
	while((SYSCTL_RCGCGPIO_R & 0x20) == 0){};
	GPIO_PORTF_AMSEL_R = (GPIO_PORTF_AMSEL_R&~0x1F);
	GPIO_PORTF_AFSEL_R = (GPIO_PORTF_AFSEL_R&~0x1F);
	GPIO_PORTF_DIR_R |= 0x0E;
	GPIO_PORTF_PCTL_R = GPIO_PORTF_PCTL_R&(~0x000FFFFF);
	GPIO_PORTF_DEN_R |= 0x1F;
  GPIO_PORTF_PUR_R |= 0x11; //Enable pullup resistors
}

#define WIDTH 160
#define HEIGHT 90
//#define DEBUG
uint16_t bufferArray[WIDTH*HEIGHT];  //Declaration of a buffer of uint16_t
buffer16 buffer;               //BufferManager struct

int main(void)
{
  //TExaS_Init();
  PLL_Init();
  PortF_Init();
  BM_BufferInit_16(&buffer,bufferArray,WIDTH,HEIGHT); //Initialize the buffer struct. It attaches your uin16 array to the struct, with w/h info
  ST7735_InitR(INITR_REDTAB);                         //Initialize the LCD. Use the appropriate argument
  ST7735_SetRotation(3);                              //Horizontal. Glorious 16:9
  BM_DrawLine_16(0,0,WIDTH-1,HEIGHT-1,0xFFFF,&buffer);
  BM_DrawLine_16(0,HEIGHT-1,WIDTH-1,0,0xFFFF,&buffer);
  //BM_DrawBitmap_16(80-27/2,45-33/2,27,33,(uint16_t*)pistol,&buffer);   //Lower left pivot. I'm going to work on a struct for bitmaps that holds all this meta data. Or incorporate it into the array itself.
  BM_ScaleBitmap_16(80-27/2,45-33/2,27,33,2000,1000,(uint16_t*)pistol,&buffer);
                                                                       //DrawBitmapOver will draw a bitmap, using 0x0000 as a transparency color
  ST7735_DrawBitmap(0,HEIGHT-1,buffer.buffer,WIDTH,HEIGHT);  //Call the hardware output. Note that I pass the BufferManager struct's array. The LCD driver looks for an array of uint16_ts
  ST7735_FillRect(0,HEIGHT,WIDTH,128-HEIGHT,0xEEEE);         //Shows where the boundary of the buffer is
  while(1){
		PF_EOR(1);  //Heartbeat
    //BM_ClearBuffer_16(&buffer);                                //Clear it to have a fresh slate to work on
  }
	return 0;
}

