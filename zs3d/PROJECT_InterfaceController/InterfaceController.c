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
#include "TExaS.h"
#include "print.h"
#include "../tm4c123gh6pm.h"
#include "../brandonware/BrandonTypes.h"
#include "../brandonware/BrandonMath.h"
#include "../brandonware/BrandonBufferManager.h"
#include "../brandonware/BrandonRaycaster.h"
#include "../brandonware/BrandonFIFO.h"

#include "../drivers/ADC.h"
#include "../drivers/UART.h"
#include "../RESOURCES/image.h"

#include "InterfaceController.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Reset(void);

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
  
#define PF_CLEAR(bit) GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R &(~(0x01<<bit))
#define PF_SET(bit) GPIO_PORTF_DATA_R |= (0x01<<bit)
#define PF_EOR(bit) GPIO_PORTF_DATA_R ^= (0x01<<bit)
#define PF_GET(bit) ((GPIO_PORTF_DATA_R & (0x01<<bit))>>bit)

#define PD_CLEAR(bit) GPIO_PORTD_DATA_R = GPIO_PORTD_DATA_R &(~(0x01<<bit))
#define PD_SET(bit) GPIO_PORTD_DATA_R |= (0x01<<bit)
#define PD_EOR(bit) GPIO_PORTD_DATA_R ^= (0x01<<bit)
#define PD_GET(bit) ((GPIO_PORTD_DATA_R & (0x01<<bit))>>bit)

#define COLUMNS 160
#define ROWS    90
#define FIFO_SIZE 2500

#define queue()   enqueue8(&fifoRX,data)                      //Because I'm a lazy fuck
#define dequeue() waitForFIFO(fifoRX);dequeue8(&fifoRX,&data) //Because I'm still a lazy fuck
//#define dequeue() dequeue8(&fifoRX,&data) //Because I'm still a lazy fuck

//My columns buffer
uint8_t rxBuffer[FIFO_SIZE];   //Larger than rayArray...to be carefull
fifo8 fifoRX;
uint16_t bufferArray[COLUMNS*ROWS];
buffer16 buffer;

#define disableAnalog() (digitalMode = 1)
uint8_t digitalMode = 0;
uint32_t center = 2048;
uint32_t deadzoneX = 100;
uint32_t deadzoneY = 100;
uint32_t adc[2];
int32_t analogOutX;
int32_t analogOutY;
uint32_t digitalOut = 0;

#define getA          (PD_GET(0)) //Positive logic
#define getB          (PD_GET(1))
#define getX          (PD_GET(2))
#define getY          (PD_GET(3))

#define getL          (PF_GET(0)^0x1) //EOR for negative logic
#define getR          (PF_GET(4)^0x1)

#define getUp         (PF_GET(0))
#define getDown       (PF_GET(0))
#define getLeft       (PF_GET(0))
#define getRight      (PF_GET(0))

////////////////////////////////

void ProcessUART(void)
{
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
      //dequeue8(&fifoRX,&data);
      data = (uint8_t)UART1_DR_R&0xFF;
      break;
    }
  }
}

uint32_t dequeueRX4Bytes()
{
  uint8_t data = 0;
  uint32_t output = 0;
  dequeue();
  output |= data<<0;
  dequeue();
  output |= data<<8;
  dequeue();
  output |= data<<16;
  dequeue();
  output |= data<<24;
  return output;
}

uint16_t dequeueRX2Bytes()
{
  uint8_t data = 0;
  uint16_t output = 0;
  dequeue();
  output |= data<<0;
  dequeue();
  output |= data<<8;
  return output;
}

// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x20; //Enable clock
	while((SYSCTL_RCGCGPIO_R & 0x20) == 0){};
  GPIO_PORTF_LOCK_R = 0x4C4F434B;       //Unlock PF7
  GPIO_PORTF_CR_R = 0xFF;               
	GPIO_PORTF_AMSEL_R = (GPIO_PORTF_AMSEL_R&~0x1F);
	GPIO_PORTF_AFSEL_R = (GPIO_PORTF_AFSEL_R&~0x1F);
	GPIO_PORTF_DIR_R = (GPIO_PORTF_DIR_R&~0x11)|(0x0E);
	GPIO_PORTF_PCTL_R = GPIO_PORTF_PCTL_R&(~0x000FFFFF);
	GPIO_PORTF_DEN_R |= 0x1F;
  GPIO_PORTF_PUR_R |= 0x11; //Enable pullup resistors
}

//PD0,1,2,3 are inputs
void PortD_Init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x08;
  while((SYSCTL_RCGCGPIO_R & 0x08) == 0);
  GPIO_PORTD_AMSEL_R = (GPIO_PORTD_AMSEL_R&~0x0F);
  GPIO_PORTD_AFSEL_R = (GPIO_PORTD_AFSEL_R&~0x0F);
  GPIO_PORTD_DIR_R = (GPIO_PORTD_DIR_R&~0x0F);
  GPIO_PORTD_PCTL_R = GPIO_PORTD_PCTL_R&(~0x0000FFFF);
  GPIO_PORTD_DEN_R |= 0x0F;
}



int main(void)
{
  TExaS_Init();
  ADC_Init();
  PortF_Init();
  PortD_Init();
	ST7735_InitR(INITR_REDTAB);
	ST7735_SetRotation(3);
  
  ST7735_DrawBitmap(0,128,splash,160,128);
  for(uint32_t temp = 0; temp <0xFFFFFF; temp++){};
  initFIFO8(&fifoRX,rxBuffer,FIFO_SIZE);
  BM_BufferInit_16(&buffer,bufferArray,COLUMNS,ROWS);
  
  UART1_Init(BAUD_921600);
  UART1_InitRXInt(UART_SEIGHTH,1,&ProcessUART);
//The great state machine
//Commands are mapped to states
#define WAIT              0x00      //Waits for STX
#define PING              0x01      //Ping for connection
#define STX               0x02      //Waits for command
#define EDX               0x03      //End transmission. Go to wait
#define RESET             0x04      //Reset
#define GET_INPUT         0x05      //Take in inputs
#define CLEAR_SCREEN      0x10      //Clear the LCD
#define LCD_DRAW_PIXEL    0x11      //
#define LCD_DRAW_LINE     0x12      //
#define LCD_FILL_RECT     0x13      //
#define LCD_INVERT        0x14      //
#define LCD_SET_CURSOR    0x15      //
#define LCD_OUTSTRING     0x16      //
#define DRAW_BUFFER       0x20      //Push buffer to display
#define CLEAR_BUFFER      0x21      //Clear buffer
#define DRAW_PIXEL        0x22      //Draw pixel
#define DRAW_LINE         0x23      //Draw line
#define DRAW_VLINE        0x24      //
#define DRAW_HLINE        0x25      //
#define FILL_RECT         0x26      //Fill a rectangle
#define DRAW_BITMAP       0x27      //Draw a bitmap
#define DRAW_BITMAP_OVER  0x28      //Draw bitmap over
#define SCALE_BITMAP      0x29      //Scale bitmap
#define SCALE_BITMAP_OVER 0x2A      //Scale bitmap over
#define DRAW_SPRITE       0x2B      //Draw a bitmap
#define DRAW_SPRITE_OVER  0x2C      //Draw bitmap over
#define SCALE_SPRITE      0x2D      //Scale bitmap
#define SCALE_SPRITE_OVER 0x2E      //Scale bitmap overs



  
  
  uint8_t state = WAIT;
  uint8_t data;
  //States
  while(1)
  {
    PF_EOR(1);
    switch(state)
    {
      case WAIT:
        dequeue();
        if(data == STX)
        {
          state = STX;
        }
        break;
      case PING:
        //Seven eighths...
        UART_OutByte(1,STX);
        UART_OutByte(1,PING);
        UART_OutByte(1,EDX);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,WAIT);
        UART_OutByte(1,STX);
        UART_OutByte(1,EDX);
        //getInput();
        state = STX;
        break;
      case STX:
        dequeue();
        state = data;
        /*
        switch(data)
        {
          case DRAW_BUFFER:
            state = data;
            break;
          case CLEAR_BUFFER:
            state = data;
            break;
          case DRAW_PIXEL:
            state = data;
            break;
          case GET_INPUT:
            state = data;
          case SEND_RAYS:
            state = data;
            break;
          case SEND_SPRITES:
            state = data;
            break;
          case SEND_PLAYER:
            state = data;
            break;
          default:
            state = WAIT;
            break;
        }
        */
        break;
      case EDX:
        state = WAIT;
        break;
      case RESET:
        Reset();
        break;
      case CLEAR_SCREEN:
        Output_Clear();
        state = STX;
        break;
      case LCD_DRAW_PIXEL:
        //lcdDrawPixel();
        state = STX;
        break;
      case LCD_DRAW_LINE:
        //lcdDrawLine();
        state = STX;
        break;
      case LCD_FILL_RECT:
        LCD_FillRect();
        state = STX;
        break;
      case LCD_INVERT:
        //lcdInvert();
        state = STX;
        break;
      case LCD_SET_CURSOR:
        LCD_SetCursor();
        state = STX;
        break;
      case LCD_OUTSTRING:
        LCD_Outstring();
        state = STX;
        break;
      case DRAW_BUFFER:
        ST7735_DrawBitmap(0,ROWS-1,buffer.buffer,COLUMNS,ROWS);
        state = STX;
        break;
      case CLEAR_BUFFER:
        BM_ClearBuffer_16(&buffer);
        state = STX;
        break;
      case DRAW_PIXEL:
        drawPixel();
        state = STX;
        break;
      case DRAW_LINE:
        drawLine();
        state = STX;
        break;
      case DRAW_VLINE:
        drawVLine();
        state = STX;
        break;
      case DRAW_HLINE:
        drawHLine();
        state = STX;
        break;
      case FILL_RECT:
        fillRect();
        state = STX;
        break;
      case DRAW_BITMAP:
        drawBitmap();
        state = STX;
        break;
      case DRAW_BITMAP_OVER:
        drawBitmapOver();
        state = STX;
        break;
      case SCALE_BITMAP:
        scaleBitmap();
        state = STX;
        break;
      case SCALE_BITMAP_OVER:
        scaleBitmapOver();
        state = STX;
        break;
      case DRAW_SPRITE:
        drawSprite();
        state = STX;
        break;
      case DRAW_SPRITE_OVER:
        drawSpriteOver();
        state = STX;
        break;
      case SCALE_SPRITE:
        scaleSprite();
        state = STX;
        break;
      case SCALE_SPRITE_OVER:
        scaleSpriteOver();
        state = STX;
        break;
      case GET_INPUT:
        getInput();
        state = STX;
        break;
      default:
        state = WAIT;
        break;
    }
  }

	return 0;
}

void Reset(void)
{
  return;
}
void drawPixel(void)
{
  uint32_t x,y;
  uint16_t color;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  color = dequeueRX2Bytes();
  BM_DrawPixel_16(x,y,color,&buffer);
}

void drawLine(void)
{
  int32_t x0,y0,x1,y1;
  uint16_t color;
  x0 = dequeueRX4Bytes();
  y0 = dequeueRX4Bytes();
  x1 = dequeueRX4Bytes();
  y1 = dequeueRX4Bytes();
  color = dequeueRX2Bytes();
  
  if(x0 > buffer.width) {x0 = buffer.width;};
  if(x0 < 0) {x0 = 0;};
  if(x1 > buffer.width) {x1 = buffer.width;};
  if(x1 < 0) {x1 = 0;};
  if(y0 > buffer.height) {y0 = buffer.height;};
  if(y0 < 0) {y0 = 0;};
  if(y1 > buffer.height) {y1 = buffer.height;};
  if(y1 < 0) {y1 = 0;};
  
  BM_DrawLine_16(x0,y0,x1,y1,color,&buffer);
}
void drawVLine(void)
{
  uint32_t x,y0,y1;
  uint16_t color;
  x = dequeueRX4Bytes();
  y0 = dequeueRX4Bytes();
  y1 = dequeueRX4Bytes();
  color = dequeueRX2Bytes();
  BM_DrawVLine_16(x,y0,y1,color,&buffer);
}
void drawHLine(void)
{
  uint32_t x0,y,x1;
  uint16_t color;
  x0 = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  x1 = dequeueRX4Bytes();
  color = dequeueRX2Bytes();
  BM_DrawVLine_16(x0,y,x1,color,&buffer);
}

void fillRect(void)
{
  int32_t x,y,w,h;
  uint16_t color;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  w = dequeueRX4Bytes();
  h = dequeueRX4Bytes();
  color = dequeueRX2Bytes();
  
  if(x > buffer.width) {x = buffer.width;};
  if(x < 0) {x = 0;};
  if(y > buffer.height) {x = buffer.height;};
  if(y < 0) {y = 0;};
  if(w > buffer.width) {w = buffer.width;};
  if(h > buffer.height) {h = buffer.height;};
  
  BM_FillRect_16(x,y,w,h,color,&buffer);
}


void drawBitmap(void)
{
  uint32_t x,y,w,h;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  w = dequeueRX4Bytes();
  h = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  BM_DrawBitmap_16(x,y,w,h,getBitmapPtr(index),&buffer);
}
void drawBitmapOver(void)
{
  uint32_t x,y,w,h;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  w = dequeueRX4Bytes();
  h = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  BM_DrawBitmapOver_16(x,y,w,h,getBitmapPtr(index),&buffer);
}
void scaleBitmap(void)
{
  uint32_t x,y,w,h;
  ufixed32_3 scaleX,scaleY;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  w = dequeueRX4Bytes();
  h = dequeueRX4Bytes();
  scaleX = dequeueRX4Bytes();
  scaleY = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  BM_ScaleBitmap_16(x,y,w,h,scaleX,scaleY,getBitmapPtr(index),&buffer);
}
void scaleBitmapOver(void)
{
  int32_t x,y,w,h;
  ufixed32_3 scaleX,scaleY;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  w = dequeueRX4Bytes();
  h = dequeueRX4Bytes();
  scaleX = dequeueRX4Bytes();
  scaleY = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  BM_ScaleBitmapOver_16(x,y,w,h,scaleX,scaleY,getBitmapPtr(index),&buffer);
}
void drawSprite(void)
{
  int32_t x,y;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  
  if(x > buffer.width) {x = buffer.width;};
  if(x < 0) {x = 0;};
  if(y > buffer.height) {x = buffer.height;};
  if(y < 0) {y = 0;};
  
  BM_DrawSprite_16(x,y,getSpritePtr(index),&buffer);
}
void drawSpriteOver(void)
{
  int32_t x,y;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  
  if(x > buffer.width) {x = buffer.width;};
  if(x < 0) {x = 0;};
  if(y > buffer.height) {x = buffer.height;};
  if(y < 0) {y = 0;};
  
  BM_DrawSpriteOver_16(x,y,getSpritePtr(index),&buffer);
}
void scaleSprite(void)
{
  int32_t x,y;
  ufixed32_3 scaleX,scaleY;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  scaleX = dequeueRX4Bytes();
  scaleY = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  
  if(x > buffer.width) {x = buffer.width;};
  if(x < 0) {x = 0;};
  if(y > buffer.height) {x = buffer.height;};
  if(y < 0) {y = 0;};
  
  if(scaleX > 10000){scaleX = 10000;};
  if(scaleY > 10000){scaleY = 10000;};
  
  BM_ScaleSprite_16(x,y,scaleX,scaleY,getSpritePtr(index),&buffer);
}
void scaleSpriteOver(void)
{
  int32_t x,y;
  ufixed32_3 scaleX,scaleY;
  uint16_t index;
  x = dequeueRX4Bytes();
  y = dequeueRX4Bytes();
  scaleX = dequeueRX4Bytes();
  scaleY = dequeueRX4Bytes();
  dequeue2Bytes(&fifoRX,&index);
  
  if(x > buffer.width) {x = buffer.width;};
  if(x < 0) {x = 0;};
  if(y > buffer.height) {x = buffer.height;};
  if(y < 0) {y = 0;};
  if(scaleX > 10000){scaleX = 10000;};
  if(scaleY > 10000){scaleY = 10000;};
  
  BM_ScaleSpriteOver_16(x,y,scaleX,scaleY,getSpritePtr(index),&buffer);
}



void getInput(void)
{
  
  //Process the joystick input
  //-1000 to 1000
  if(digitalMode == 0)
  {
    ADC_In(adc);
    int32_t x = adc[1];
    int32_t y = adc[0];
    
    /*
    if(x < (center-deadzoneX))
    {
      analogOutX = (-1000*((center-deadzoneX)-x))/center;
    }
    else if( x > (center+deadzoneX))
    {
      analogOutX = (1000 * (x-(center+deadzoneX)))/center;
    }
    else
    {
      analogOutX = 0;
    }
    
    if(y < (center-deadzoneY))
    {
      analogOutY = (-1000 * ((center-deadzoneY)-y))/center;
    }
    else if( y > (center+deadzoneX))
    {
      analogOutY = (1000 * (y-(center+deadzoneY)))/center;
    }
    else
    {
      analogOutY = 0;
    */
    
    
    if(x < (center-deadzoneX) || x > (center+deadzoneX))
    {
      analogOutX = x - center;
    }
    else
    {
      analogOutX = 0;
    }
    
    if(y < (center-deadzoneX) || y > (center+deadzoneX))
    {
      analogOutY = y - center;
    }
    else
    {
      analogOutY = 0;
    }
    
  }
  else
  {
    analogOutX = getLeft?-1000:0;
    analogOutX = getRight?1000:0;
    analogOutY = getDown?-1000:0;
    analogOutY = getUp?1000:0;
  }
  //DIGITAL SHIT
  digitalOut = 0;
  digitalOut |= getA<<0;
  digitalOut |= getB<<1;
  digitalOut |= getX<<2;
  digitalOut |= getY<<3;
  digitalOut |= getL<<4;
  digitalOut |= getR<<5;
  
  UART_OutByte(1,STX);
  UART_OutByte(1,GET_INPUT);
  UART_Out4Bytes(1,digitalOut);
  UART_Out4Bytes(1,analogOutX);
  UART_Out4Bytes(1,analogOutY);
  UART_OutByte(1,EDX);
}

void LCD_FillRect(void)
{
  uint16_t x,y,w,h,color;
  dequeue2Bytes(&fifoRX,&x);
  dequeue2Bytes(&fifoRX,&y);
  dequeue2Bytes(&fifoRX,&w);
  dequeue2Bytes(&fifoRX,&h);
  dequeue2Bytes(&fifoRX,&color);
  ST7735_FillRect(x,y,w,h,color);
}

void LCD_SetCursor(void)
{
  uint32_t x,y;
  dequeue4Bytes(&fifoRX,&x);
  dequeue4Bytes(&fifoRX,&y);
  ST7735_SetCursor(x,y);
}

void LCD_Outstring(void)
{
  uint8_t current;
  dequeue8(&fifoRX,&current);
  while (current != 0)
  {
    ST7735_OutChar(current);
    dequeue8(&fifoRX,&current);
  }
  return;
}

void RenderWallTexture(void)
{
  //100x100 texture
  uint32_t x;                                       //x coordinate
  uint32_t xOff;
  uint32_t height;
  uint8_t textureIndex;
  dequeue4Bytes(&fifoRX,&x);
  dequeue4Bytes(&fifoRX,&xOff);
  dequeue4Bytes(&fifoRX,&height);
  dequeue8(&fifoRX,&textureIndex);
  int32_t xT = xOff/10;                                //X offset in texture
  uint16_t* texPtr = getTexturePtr(textureIndex);   //Point to texture
  //Ryan's scale bitmap may have a clue
}