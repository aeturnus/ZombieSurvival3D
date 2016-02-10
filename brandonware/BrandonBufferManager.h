#ifndef __BRANDONBUFFERMANAGER_H__
#define __BRANDONBUFFERMANAGER_H__

#include <stdint.h>
#include "BrandonTypes.h"

#define BM_DrawPixel(x,y,color,b) if(x < b->width && y < b->height && x >=0 && y >= 0){b->buffer[x + y * (b->width)]=color;}

typedef struct BM_Sprite16
{
  uint16_t width;
  uint16_t height;
  uint16_t pivotX;
  uint16_t pivotY;
  uint16_t* bitmap;
} sprite16;

typedef struct BM_Buffer8
{
	uint8_t* buffer;
	uint32_t width;
	uint32_t height;
} buffer8;
typedef struct BM_Buffer16
{
	uint16_t* buffer;
	uint32_t width;
	uint32_t height;
} buffer16;
typedef struct BM_Buffer32
{
	uint32_t* buffer;
	uint32_t width;
	uint32_t height;
} buffer32;


/* Functions */
// These subroutines are meant for 16 bit buffers

// ----- Buffer Init -----
// Inputs: pointer to buffer object, pointer to array for buffer, buffer width, buffer height
// Outputs: none
// This will initialize a buffer with the given parameers
void BM_BufferInit_16(buffer16* buffer, uint16_t* bufferArray, uint32_t bufferW, uint32_t bufferH);

// ----- Draw Pixel -----
// Inputs: x, y, color, buffer info
// Outputs: none
// This will navigate the buffer and place a 16 bit color into that cell
// This is a "master" function due to the array based nature of the buffer
// This automatically culls points not displayable (becaue I don't want to ruin memory)
void BM_DrawPixel_16(int32_t x, int32_t y, uint16_t color, buffer16* b);

// ----- Get Pixel -----
// Inputs: x, y, buffer info
// Outputs: none
// This will navigate the buffer and retreive a 16 bit color from that cell

uint16_t BM_GetPixel_16(int32_t x, int32_t y, buffer16* b);

// ----- Draw Bitmap -----
// Inputs: x,y,w,h,buffer,pointer to bitmap
// Outputs: none
// This will put a stripped bitmap (as per valvanoware) into the buffer at the requested location
void BM_DrawBitmap_16(int32_t x, int32_t y, uint32_t width, uint32_t height, uint16_t* bitmapPtr, buffer16* b);

// ----- Draw Bitmap Over -----
// Inputs: x,y,w,h,buffer,pointer to bitmap
// Outputs: none
// This will put a stripped bitmap (as per valvanoware) into the buffer at the requested location. Takes 0x0000 as a transparency
void BM_DrawBitmapOver_16(int32_t x, int32_t y, uint32_t width, uint32_t height, uint16_t* bitmapPtr, buffer16* b);

// ----- Draw Bitmap Invert -----
// Inputs: x,y,w,h,buffer,pointer to bitmap
// Outputs: none
// This will put a stripped bitmap (as per valvanoware) into the buffer, inverting colors present. 0x0000 is ignored
void BM_DrawBitmapInvert_16(int32_t x, int32_t y, uint32_t width, uint32_t height, uint16_t* bitmapPtr, buffer16* b);

// ----- Draw Sprite Over-----
// Inputs: x,y,sprite16 pointer, buffer pointer
// Output: none
// This will draw a sprite struct
void BM_DrawSprite_16(int32_t x, int32_t y, sprite16* spritePtr, buffer16* bufferPtr);

// ----- Draw Sprite -----
// Inputs: x,y,sprite16 pointer, buffer pointer
// Output: none
// This will draw a sprite struct
void BM_DrawSpriteOver_16(int32_t x, int32_t y, sprite16* spritePtr, buffer16* bufferPtr);

// ----- Draw Sprite -----
// Inputs: x,y,sprite16 pointer, buffer pointer
// Output: none
// This will draw a sprite struct
void BM_DrawSpriteInvert_16(int32_t x, int32_t y, sprite16* spritePtr, buffer16* bufferPtr);


///////DO THIS RYAN


// ---- Scale Bitmap -----
// Inputs : x,y,w,h,scale,buffer,pointer to bitmap
// Outputs: none
// x and y are locations on the buffer on where to draw final product
// Will scale a bitmap, putting it into the buffer at a certain scale
void BM_ScaleBitmap_16(int32_t x, int32_t y, uint32_t width, uint32_t height, ufixed32_3 scaleX, ufixed32_3 scaleY, uint16_t* bitmapPtr, buffer16* b);

// ---- Scale Bitmap Over -----
// Inputs : x,y,w,h,scale,buffer,pointer to bitmap
// Outputs: none
// Will scale a bitmap, putting it into the buffer at a certain scale. Takes 0x0000 as a transparency
void BM_ScaleBitmapOver_16(int32_t x, int32_t y, uint32_t width, uint32_t height, ufixed32_3 scaleX, ufixed32_3 scaleY, uint16_t* bitmapPtr, buffer16* b);

// ---- Scale Sprite -----
// Inputs : x,y,w,h,scale,buffer,pointer to sprite
// Outputs: none
// x and y are locations on the buffer on where to draw final product
// Will scale a bitmap, putting it into the buffer at a certain scale
void BM_ScaleSprite_16(int32_t x, int32_t y,ufixed32_3 scaleX, ufixed32_3 scaleY, sprite16* spritePtr, buffer16* b);

// ---- Scale Sprite Over -----
// Inputs : x,y,w,h,scale,buffer,pointer to sprite
// Outputs: none
// Will scale a bitmap, putting it into the buffer at a certain scale. Takes 0x0000 as a transparency
void BM_ScaleSpriteOver_16(int32_t x, int32_t y,ufixed32_3 scaleX, ufixed32_3 scaleY, sprite16* spritePtr, buffer16* b);


////////DO THIS RYAN


// ----- Fill Rectangle -----
// Inputs : x,y,w,h,color,buffer info
// Outputs: none
// This will fill a rectangle with a certain color. The x and y position count as pixels.
// Aligned to the bottom left corner of the matrix. Your mileage may vary depending on the device you're using to display this array
void BM_FillRect_16(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color, buffer16* b);


// ----- Draw Column -----
// Inputs : x,y,h,color,buffer info
// Outputs: none
// This will draw a line from (x,y) to (x,y+h)
// Intended to be a fast way to draw non diagonals
void BM_DrawColumn_16(int32_t x, int32_t y, int32_t h, uint16_t color, buffer16* b);

// ----- Draw Row -----
// Inputs : x,y,w,color,buffer info
// Outputs: none
// This will draw a line from (x,y) to (x+w,y)
// Intended to be a fast way to draw non diagonals
void BM_DrawRow_16(int32_t x, int32_t y, int32_t w, uint16_t color, buffer16* b);

// ----- Draw Line -----
// Inputs : x1,y1,x2,y2,color,buffer info
// Outputs: none
// This will draw a line from (x1,y1) to (x2,y2) with the color using Bresnehem's algorithm
// This is taken from the Nokia5110_Functions.h
void BM_DrawLine_16(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color, buffer16* b);

void BM_DrawVLine_16(int32_t x, int32_t y0, int32_t y1, uint16_t color, buffer16* bufferPtr);
void BM_DrawHLine_16(int32_t x0, int32_t y, int32_t x1, uint16_t color, buffer16* bufferPtr);

// ----- Draw Line Thick -----
// Inputs : x1,y1,x2,y2,color,thickness,buffer info
// Outputs: none
// Draw a thick line
void BM_DrawLineThick_16(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color, uint8_t thickness, buffer16* b);

// ----- Draw Triangle-----
// Inputs : x1,y1,x2,y2,x3,y3,color,buffer info
// Outputs: none
// Draws a triangle given its points
void BM_DrawTriangle_16(int32_t p1x, int32_t p1y, int32_t p2x, int32_t p2y, int32_t p3x, int32_t p3y,uint16_t color, buffer16* buffer);

// ----- Color Darken -----
// Inputs : 565 16bit color, number of times to brighten
// Outputs: None
// This will darken a color given the number of times
uint16_t BM_ColorDarken_16(uint16_t color, uint8_t times);

// ----- Color Brighten -----
// Inputs : 565 16bit color, number of times to brighten
// Outputs: None
// This will brighten a color given the number of times
uint16_t BM_ColorBrighten_16(uint16_t color, uint8_t times);

// ----- Clear Buffer -----
// Inputs : Buffer info
// Outputs: None
// This will call a fill rect that will fill everything with color 0x000
void BM_ClearBuffer_16(buffer16* b);

#endif
