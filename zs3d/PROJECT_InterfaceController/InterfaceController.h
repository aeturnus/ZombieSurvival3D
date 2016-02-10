#ifndef __INTERFACE_H__
#define __INTERFACE_H__

void drawPixel(void);
void drawLine(void);
void fillRect(void);
void drawBitmap(void);
void drawBitmapOver(void);
void scaleBitmap(void);
void scaleBitmapOver(void);
void drawSprite(void);
void drawSpriteOver(void);
void scaleSprite(void);
void scaleSpriteOver(void);
void getInput(void);

void drawVLine(void);
void drawHLine(void);

void LCD_FillRect(void);
void LCD_SetCursor(void);
void LCD_Outstring(void);
#endif
