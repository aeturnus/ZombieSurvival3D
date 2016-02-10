
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: put your names here
// Date Created: 1/24/2015 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "../../tm4c123gh6pm.h"
#include "../TExaS.h"
#include "../../drivers/dac.h"
#include "../../drivers/Sound.h"
#include "../../drivers/UART.h"
#include "../../drivers/SysTick.h"
#include "../../drivers/Timer0.h"
#include "../../brandonware/BrandonMML.h"
#include "../RESOURCES/songs.h"
#include "../RESOURCES/sounds.h"
#include "../../brandonware/BrandonFIFO.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts



#define FIFO_SIZE 1024
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF_CLEAR(bit) GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R &(~(0x01<<bit))
#define PF_SET(bit) GPIO_PORTF_DATA_R |= (0x01<<bit)
#define PF_EOR(bit) GPIO_PORTF_DATA_R ^= (0x01<<bit)
#define PF_GET(bit) ((GPIO_PORTF_DATA_R & (0x01<<bit))>>bit)
uint8_t fifoRXBuffer [FIFO_SIZE];
fifo8 fifoRX;

struct BMML_Track tracks[2];
volatile struct BMML_TrackHolder trackHolder = {2,tracks,0,0};
//void SysTick_Handler()

void HandleTimer0()
{
  Sound_Out();
}
void HandleSysTick()
{
  //SysTick_DisableInt();
  BMML_HolderUpdate(&trackHolder,SysTick_Reload);
  DAC_Out(trackHolder.output);
  PF_EOR(3);
  //SysTick_EnableInt();
}

void HandleUART()
{
  //This will dequeue the HW fifo and dump it into the sw fifo
  PF_EOR(2);
  uint8_t data;
  while((UART1_FR_R & UART_FR_RXFE) == 0)   //While the H/W FIFO is not empty, pass the H/W FIFO contents into the S/W FIFO
  {
    if(fifoRX.status != FIFO_FULL)            //Check FIFO status
    {
      data = (uint8_t)UART1_DR_R&0xFF;      //Get value off the H/W queue 
      enqueue8(&fifoRX,data);               //enqueue it into the S/W FIFO
    }
    else
    {
      break;
    }
  }
}
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


int main(void){      
  //TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  PLL_Init();
  //TExaS_Init(NoLCD_NoScope);
  DisableInterrupts();
  
  initFIFO8(&fifoRX,fifoRXBuffer,FIFO_SIZE);
  DAC_Init();
  PortF_Init();
  PF_SET(1);
  PF_CLEAR(2);
  PF_CLEAR(3);
  // other initialization
  SysTick_Init((80000000/(2000*96)),2,&HandleSysTick);
  Timer0_Init(&HandleTimer0, 7256);
  BMML_HolderInit(&trackHolder,NVIC_ST_RELOAD_R);
  Sound_Init((sound*)soundsList);
  DAC_SetEffect(0xFFFFFFFF);
  SysTick_DisableAll();
  SysTick_Clear();
  UART1_Init(BAUD_115200);
  UART1_InitRXInt(UART_EIGHTH,1,&HandleUART);
  
  EnableInterrupts();
  
  PF_SET(2);
  PF_SET(3);
  //Commands
  #define WAIT        0x00    //Waits for a STX
  #define STX         0x02     //Transmission started. Hunts for a command
  #define EDX         0x03     //Transmission end. Will go to WAIT
  #define M_RESET     0x10
  #define LOAD_SONG   0x11
  #define PAUSE_SONG  0x12
  #define PLAY_SONG   0x13
  #define PLAY_SOUND  0x14
  uint8_t state = WAIT;
  uint8_t data;
	while(1)
  {
    switch(state)
    {
      PF_EOR(3);
      case WAIT:
        while(fifoRX.status == FIFO_EMPTY)
        {
        }
        dequeue8(&fifoRX,&data);
        if(data == 0x02)
        {
          state = STX;
        }
        break;
      case STX:
        while(fifoRX.status == FIFO_EMPTY)
        {
        }
        dequeue8(&fifoRX,&data);
        state = data;
        break;
      case EDX:
        state = WAIT;
        break;
      case M_RESET:
        main();
        break;
      case LOAD_SONG:
        while(fifoRX.status == FIFO_EMPTY)
        {
        }
        dequeue8(&fifoRX,&data);
        BMML_HolderLoadSong(&trackHolder,(song_t*)&songArray[data],NVIC_ST_RELOAD_R);
        state = STX;
        break;
      case PAUSE_SONG:
        SysTick_DisableAll();
        state = STX;
        break;
      case PLAY_SONG:
        SysTick_EnableAll();
        state = STX;
        break;
      case PLAY_SOUND:
        dequeue8(&fifoRX,&data);
        Sound_Load(data);
        Sound_Play();
        state = STX;
        break;
    }
  }
}
