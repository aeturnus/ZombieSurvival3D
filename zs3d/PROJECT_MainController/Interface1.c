#include "Interface1.h"
#include "../../drivers/UART.h"
#include "../../brandonware/BrandonFIFO.h"
#include "../../brandonware/BrandonTypes.h"

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

#define outchar(a) UART3_OutChar(a)
#define MODULE 3

#define dequeue() if(fifoRX.status == FIFO_EMPTY){trans=0;break;};dequeue8(&fifoRX,&data)

#define FIFO_SIZE 128
uint8_t fifoBuffer[FIFO_SIZE];
fifo8 fifoRX;
  
uint32_t state = 0;
uint8_t data;

uint8_t present = 0;
int32_t analogX,analogY;
uint8_t  digitalA,digitalB,digitalX,digitalY,digitalL,digitalR;

void getInput(void);
  
void processState(void)
{
  uint8_t trans = 1;
  while(trans)
  {
    switch(state)
    {
      case WAIT:
        dequeue();
        if(data == STX)
        {
          state = STX;
        }
        break;
      case STX:
        dequeue();
        state = data;
        break;
      case EDX:
        state = WAIT;
        trans = 0;
        break;
      case PING:
        present = 1;
        state = STX;
        break;
      case GET_INPUT:
        getInput();
        state = STX;
        break;     
    }
  }
}
void Interface1_Handler(void)
{
  uint8_t uartData;
  while((UART3_FR_R & UART_FR_RXFE) == 0)   //While the H/W FIFO is not empty, pass the H/W FIFO contents into the S/W FIFO
  {
    if(fifoRX.status != FIFO_FULL)            //Check FIFO status
    {
      uartData = (uint8_t)UART3_DR_R&0xFF;      //Get value off the H/W queue 
      enqueue8(&fifoRX,uartData);               //enqueue it into the S/W FIFO
    }
    else
    {
      break;
    }
  }
  processState();
}

void Interface1_Init(void)
{
  initFIFO8(&fifoRX,fifoBuffer,FIFO_SIZE);
  UART3_Init(BAUD_921600);
  UART3_InitRXInt(UART_SEIGHTH,1,&Interface1_Handler);
}
void Interface1_Reset(void)
{
  outchar(STX);
  outchar(RESET);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_Ping(void)
{
  outchar(STX);
  UART_OutByte(MODULE,PING);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

uint8_t Interface1_IsPresent(void)
{
  return present;
}

void Interface1_ClearScreen(void)
{
  outchar(STX);
  outchar(CLEAR_SCREEN);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_DrawBuffer(void)
{
  outchar(STX);
  outchar(DRAW_BUFFER);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_ClearBuffer(void)
{
  outchar(STX);
  outchar(CLEAR_BUFFER);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_DrawPixel(uint32_t x, uint32_t y, uint16_t color)
{
  outchar(STX);
  outchar(DRAW_PIXEL);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out2Bytes(MODULE,color);
  outchar(EDX);
}

void Interface1_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color)
{
  outchar(STX);
  outchar(DRAW_LINE);
  UART_Out4Bytes(MODULE,x0);
  UART_Out4Bytes(MODULE,y0);
  UART_Out4Bytes(MODULE,x1);
  UART_Out4Bytes(MODULE,y1);
  UART_Out2Bytes(MODULE,color);
  outchar(EDX);
}

void Interface1_FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color)
{
  outchar(STX);
  outchar(FILL_RECT);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,w);
  UART_Out4Bytes(MODULE,h);
  UART_Out2Bytes(MODULE,color);
  outchar(EDX);
}

void Interface1_GetInput(void)
{
  outchar(STX);
  outchar(GET_INPUT);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

int32_t Interface1_GetAnalog(uint8_t axis)
{
  switch(axis)
  {
    case 'x':
      return analogX;
      break;
    case 'y':
      return analogY;
      break;
  }
  return 0;
}

uint8_t Interface1_GetDigital(uint8_t button)
{
  switch(button)
  {
    case 'a':
      return digitalA;
      break;
    case 'b':
      return digitalB;
      break;
    case 'x':
      return digitalX;
      break;
    case 'y':
      return digitalY;
      break;
    case 'l':
      return digitalL;
      break;
    case 'r':
      return digitalR;
      break;
  }
  return 0;
}


void Interface1_DrawBitmap(int32_t x,int32_t y, int32_t w, int32_t h, uint16_t index)
{
  outchar(STX);
  outchar(DRAW_BITMAP);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,w);
  UART_Out4Bytes(MODULE,h);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}
void Interface1_DrawBitmapOver(int32_t x,int32_t y, int32_t w, int32_t h, uint16_t index)
{
  outchar(STX);
  outchar(DRAW_BITMAP_OVER);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,w);
  UART_Out4Bytes(MODULE,h);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}
void Interface1_ScaleBitmap(int32_t x,int32_t y, int32_t w, int32_t h, ufixed32_3 scaleX, ufixed32_3 scaleY, uint16_t index)
{
  outchar(STX);
  outchar(SCALE_BITMAP);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,w);
  UART_Out4Bytes(MODULE,h);
  UART_Out4Bytes(MODULE,scaleX);
  UART_Out4Bytes(MODULE,scaleY);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}
void Interface1_ScaleBitmapOver(int32_t x,int32_t y, int32_t w, int32_t h, ufixed32_3 scaleX, ufixed32_3 scaleY, uint16_t index)
{
  outchar(STX);
  outchar(SCALE_BITMAP_OVER);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,w);
  UART_Out4Bytes(MODULE,h);
  UART_Out4Bytes(MODULE,scaleX);
  UART_Out4Bytes(MODULE,scaleY);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}
void Interface1_DrawSprite(int32_t x,int32_t y,uint16_t index)
{
  outchar(STX);
  outchar(DRAW_SPRITE);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}
void Interface1_DrawSpriteOver(int32_t x,int32_t y,uint16_t index)
{
  outchar(STX);
  outchar(DRAW_SPRITE_OVER);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}
void Interface1_ScaleSprite(int32_t x,int32_t y,ufixed32_3 scaleX, ufixed32_3 scaleY,uint16_t index)
{
  outchar(STX);
  outchar(SCALE_SPRITE);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,scaleX);
  UART_Out4Bytes(MODULE,scaleY);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}
void Interface1_ScaleSpriteOver(int32_t x,int32_t y,ufixed32_3 scaleX, ufixed32_3 scaleY,uint16_t index)
{
  outchar(STX);
  outchar(SCALE_SPRITE_OVER);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,scaleX);
  UART_Out4Bytes(MODULE,scaleY);
  UART_Out2Bytes(MODULE,index);
  outchar(EDX);
}


void getInput(void)
{
  uint32_t digital;
  uint32_t analog;
  dequeue4Bytes(&fifoRX,&digital);
  dequeue4Bytes(&fifoRX,(uint32_t*)&analogX);
  //analogX = (int32_t)analog;
  dequeue4Bytes(&fifoRX,(uint32_t*)&analogY);
  //analogY = (int32_t)analog;
  digitalA = digital&0x1;
  digital = digital>>1;
  digitalB = digital&0x1;
  digital = digital>>1;
  digitalX = digital&0x1;
  digital = digital>>1;
  digitalY = digital&0x1;
  digital = digital>>1;
  digitalL = digital&0x1;
  digital = digital>>1;
  digitalR = digital&0x1;
  return;
}

void Interface1_LCDFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  outchar(STX);
  outchar(LCD_FILL_RECT);
  UART_Out2Bytes(MODULE,x);
  UART_Out2Bytes(MODULE,y);
  UART_Out2Bytes(MODULE,w);
  UART_Out2Bytes(MODULE,h);
  UART_Out2Bytes(MODULE,color);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_LCDSetCursor(uint32_t x, uint32_t y)
{
  outchar(STX);
  outchar(LCD_SET_CURSOR);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_LCDOutString(uint8_t* string)
{
  outchar(STX);
  outchar(LCD_OUTSTRING);
  while(*string != 0x00)
  {
    outchar(*string);
    string++;
  }
  outchar(0x00);
  outchar(EDX);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
  outchar(0x00);
 outchar(0x00);
  outchar(0x00);
  outchar(0x00);
}

void Interface1_DrawVLine(uint32_t x, uint32_t y0, uint32_t y1, uint16_t color)
{
  x = x>=160?160:x;
  y0 = y0>=160?160:y0;
  y1 = y1>=160?160:y1;
  outchar(STX);
  outchar(DRAW_VLINE);
  UART_Out4Bytes(MODULE,x);
  UART_Out4Bytes(MODULE,y0);
  UART_Out4Bytes(MODULE,y1);
  UART_Out2Bytes(MODULE,color);
  outchar(EDX);
}
void Interface1_DrawHLine(uint32_t x0, uint32_t y, uint32_t x1, uint16_t color)
{
  x0 = x0>=160?160:x0;
  x1 = x1>=160?160:x1;
  y = y>=160?160:y;
  outchar(STX);
  outchar(DRAW_HLINE);
  UART_Out4Bytes(MODULE,x0);
  UART_Out4Bytes(MODULE,y);
  UART_Out4Bytes(MODULE,x1);
  UART_Out2Bytes(MODULE,color);
  outchar(EDX);
}