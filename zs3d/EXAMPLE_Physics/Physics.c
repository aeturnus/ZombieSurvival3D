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
#include "../../brandonware/BrandonPhysics.h"
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
	NVIC_ST_RELOAD_R = 80000;							      //Reload value: .001 ms interrupts
	NVIC_ST_CURRENT_R = 0;											//Clear current
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;		//Priority 1
	NVIC_ST_CTRL_R = 0x0007;										//Enable SysTick w/ core clock and int
}

//Physics timer
fixed32_3 deltaT = 0; //in fixed point seconds
void SysTick_Handler(void)
{
  deltaT++; //increment for since 1 represents a millisecond
}
uint8_t doneDrawing = 0;  //here's a flag to skip a frame. set it when you're done drawing to the buffer. This'll tell if you if you should send the buffer to the lcd
//Feel free to use it to keep a constant framerate. TBH i'd use a timer. I'll update this later perhaps

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


#define NUM_ENTITIES 2
entity entityList[NUM_ENTITIES];

int main(void)
{
  //TExaS_Init();
  PLL_Init();
  PortF_Init();
  SysTick_Init();
  BM_BufferInit_16(&buffer,bufferArray,WIDTH,HEIGHT); //Initialize the buffer struct. It attaches your uin16 array to the struct, with w/h info
  ST7735_InitR(INITR_REDTAB);                         //Initialize the LCD. Use the appropriate argument
  ST7735_SetRotation(3);                              //Horizontal. Glorious 16:9
  ST7735_DrawBitmap(0,HEIGHT-1,buffer.buffer,WIDTH,HEIGHT);  //Call the hardware output. Note that I pass the BufferManager struct's array. The LCD driver looks for an array of uint16_ts
  ST7735_FillRect(0,HEIGHT,WIDTH,128-HEIGHT,0xEEEE);         //Shows where the boundary of the buffer is
  entity* ent = &entityList[0];
  ent->pos.x = -200000;
  ent->pos.y = 70000;
  ent->pos.z = 0;
  ent->vel.x = 200000;
  ent->vel.y = 0;
  ent->vel.z = 0;
  ent->acc.x = 0;
  ent->acc.y = 0;
  ent->acc.z = 0;
  ent->dim.width = 11000;
  ent->dim.height = 11000;
  
  ent = &entityList[1];
  ent->pos.x = 70000;
  ent->pos.y = 70000;
  ent->pos.z = 0;
  ent->vel.x = 0;
  ent->vel.y = 0;
  ent->vel.z = 0;
  ent->acc.x = 0;
  ent->acc.y = 0;
  ent->acc.z = 0;
  ent->dim.width = 11000;
  ent->dim.height = 11000;
  
  while(1){
		PF_EOR(1);  //Heartbeat
    BM_ClearBuffer_16(&buffer);
    
    //
    fixed32_3 dT = deltaT;
    deltaT = 0;
    uint8_t k,l,s;
    for(k=0;k<NUM_ENTITIES;k++)
    {
        PH_EntityAct(&entityList[k],dT);
    }
    entity* ptr0;
    entity* ptr1;
    
    //Since k[0] already checks everyone else, no one has to check k[0] intercept again. And so on for k[1]!! OPTIMIZE!
    for(k=0,s=0;k<NUM_ENTITIES;k++,s++)
    {
      for(l=s;l<NUM_ENTITIES;l++)
      {
        ptr0 = &entityList[k];
        ptr1 = &entityList[l];
        if(ptr0 != ptr1)
        {
          ST7735_SetCursor(15,11);
          ST7735_OutString("     ");
          if(PH_CheckCollision(ptr0,ptr1))
          {
            ST7735_SetCursor(15,11);
            ST7735_OutString("Bang!");
          }
        }
      }
    }
    
    //BM_DrawBitmap_16(toDec_3(ent->pos.x)-5,toDec_3(ent->pos.y)-5,11,11,(uint16_t*)crosshair,&buffer);
    for(k=0;k<NUM_ENTITIES;k++)
    {
        BM_DrawBitmapOver_16(toDec_3(entityList[k].pos.x)-5,toDec_3(entityList[k].pos.y)-5,11,11,(uint16_t*)crosshair,&buffer);
    }
    
    ST7735_DrawBitmap(0,HEIGHT-1,buffer.buffer,WIDTH,HEIGHT);  //Call the hardware output. Note that I pass the BufferManager struct's array. The LCD driver looks for an array of uint16_ts
    ST7735_SetCursor(0,11);
    ST7735_OutString("        ");
    ST7735_SetCursor(0,11);
    LCD_OutDec(dT);
  }
	return 0;
}

