//Brandon Nguyen

#include <stdint.h>
#include "SysTick.h"
#include "tm4c123gh6pm.h"

void (*SysTick_Task)(void);
void SysTick_Init(uint32_t period,uint8_t priority,void(*task)(void))
{
  //NVIC_ST_CTRL_R = 0;                         //Disable SysTick during init
  SysTick_Disable();
  //NVIC_ST_RELOAD_R = period-1;                //Reload value
  SysTick_ChangeReload(period-1);
  //NVIC_ST_CURRENT_R = 0;                      //Clear current
  SysTick_Clear();
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x07FFFFFF)|priority<<29;  //set priority of interrupt
  SysTick_ChangeHandler(task);
  //NVIC_ST_CTRL_R = 0x0007;                    //Enable SysTick w/ core clock and int1
  SysTick_EnableInt();
  SysTick_Enable();
}
void SysTick_ChangeHandler(void(*task)(void))
{
  SysTick_Task = task;
}
void SysTick_Handler(void)
{
  SysTick_Task();
}
/*
void SysTick_Enable(void)
{
  NVIC_ST_CTRL_R = 0x0007;
}
void SysTick_Disable(void)
{
  NVIC_ST_CTRL_R = 0x0000;
}
*/
