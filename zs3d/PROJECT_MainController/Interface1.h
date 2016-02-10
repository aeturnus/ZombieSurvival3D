#ifndef __INTERFACE1_H__
#define __INTERFACE1_H__

#include <stdint.h>
#include "../../brandonware/BrandonTypes.h"

//Commands are mapped to states
/*
 * UART 3
 * TX: PC7
 * RX: PC6
 */

void Interface1_Init(void);
void Interface1_Reset(void);
void Interface1_Ping(void);
uint8_t Interface1_IsPresent(void);
void Interface1_ClearScreen(void);
void Interface1_DrawBuffer(void);
void Interface1_ClearBuffer(void);
void Interface1_DrawPixel(uint32_t x, uint32_t y, uint16_t color);
void Interface1_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);
void Interface1_FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color);
void Interface1_GetInput(void);
void Interface1_DrawBitmap(int32_t x,int32_t y, int32_t w, int32_t h, uint16_t index);
void Interface1_DrawBitmapOver(int32_t x,int32_t y, int32_t w, int32_t h, uint16_t index);
void Interface1_ScaleBitmap(int32_t x,int32_t y, int32_t w, int32_t h, ufixed32_3 scaleX, ufixed32_3 scaleY, uint16_t index);
void Interface1_ScaleBitmapOver(int32_t x,int32_t y, int32_t w, int32_t h, ufixed32_3 scaleX, ufixed32_3 scaleY, uint16_t index);
void Interface1_DrawSprite(int32_t x,int32_t y,uint16_t index);
void Interface1_DrawSpriteOver(int32_t x,int32_t y,uint16_t index);
void Interface1_ScaleSprite(int32_t x,int32_t y,ufixed32_3 scaleX, ufixed32_3 scaleY,uint16_t index);
void Interface1_ScaleSpriteOver(int32_t x,int32_t y,ufixed32_3 scaleX, ufixed32_3 scaleY,uint16_t index);

void Interface1_LCDFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void Interface1_LCDSetCursor(uint32_t x, uint32_t y);
void Interface1_LCDOutString(uint8_t* string);


void Interface1_DrawVLine(uint32_t x, uint32_t y0, uint32_t y1, uint16_t color);
void Interface1_DrawHLine(uint32_t x0, uint32_t y, uint32_t x1, uint16_t color);

int32_t Interface1_GetAnalog(uint8_t axis);
uint8_t Interface1_GetDigital(uint8_t button);

#endif
