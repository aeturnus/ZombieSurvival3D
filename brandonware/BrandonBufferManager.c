#include <stdint.h>
#include "BrandonBufferManager.h"
#include "BrandonTypes.h"
#include "BrandonMath.h"

// These subroutines are meant for 16 bit buffers

// ----- Buffer Init -----
// Inputs: pointer to buffer object, pointer to array for buffer, buffer width, buffer height
// Outputs: none
// This will initialize a buffer with the given parameers

void BM_BufferInit_16(buffer16* buffer, uint16_t* bufferArray, uint32_t bufferW, uint32_t bufferH)
{
  buffer->buffer = bufferArray;
  buffer->width = bufferW;
  buffer->height = bufferH;
}

// ----- Draw Pixel -----
// Inputs: x, y, color, buffer info
// Outputs: none
// This will navigate the buffer and place a 16 bit color into that cell
// This is a "master" function due to the array based nature of the buffer
// This automatically culls points not displayable (becaue I don't want to ruin memory)


void BM_DrawPixel_16(int32_t x, int32_t y, uint16_t color, buffer16* b)
{
  //If the x and y positions are outside the bounds of the buffer, ignore the pixel
  if((x >= b->width) || (y >= b->height) || (x < 0) || (y < 0)) 
  {
    return;
  }
  int32_t index = x + y * (b->width); //Calculate offset inside the buffer to put the pixel
  b->buffer[index] = color;
}


// ----- Get Pixel -----
// Inputs: x, y, buffer info
// Outputs: none
// This will navigate the buffer and retreive a 16 bit color from that cell

uint16_t BM_GetPixel_16(int32_t x, int32_t y, buffer16* b)
{
  //If the x and y positions are outside the bounds of the buffer, ignore the pixel
  int32_t index = x + y * (b->width); //Calculate offset inside the buffer to put the pixel
  return b->buffer[index];
}


// ----- Draw Bitmap -----
// Inputs: x,y,w,h,buffer,pointer to bitmap
// Outputs: none
// This will put a stripped bitmap (as per valvanoware) into the buffer at the requested location. Bottom left is the origin
void BM_DrawBitmap_16(int32_t x, int32_t y, uint32_t width, uint32_t height, uint16_t* bitmapPtr, buffer16* b)
{
  uint32_t r,c;
  for(r = 0; r<height; r++)
  {
    for(c = 0; c<width; c++)
    {
      BM_DrawPixel_16(x+c,y+r,*bitmapPtr,b);
      bitmapPtr++;
    }
  }
}

// ----- Draw Bitmap Over -----
// Inputs: x,y,w,h,buffer,pointer to bitmap
// Outputs: none
// This will put a stripped bitmap (as per valvanoware) into the buffer at the requested location. Takes 0x0000 as a transparency
void BM_DrawBitmapOver_16(int32_t x, int32_t y, uint32_t width, uint32_t height, uint16_t* bitmapPtr, buffer16* b)
{
  uint32_t r,c;
  for(r = 0; r<height; r++)
  {
    for(c = 0; c<width; c++)
    {
      if(*bitmapPtr != 0x0000)
      {
        BM_DrawPixel_16(x+c,y+r,*bitmapPtr,b);
      }
      bitmapPtr++;
    }
  }
}

// ----- Draw Bitmap Invert -----
// Inputs: x,y,w,h,buffer,pointer to bitmap
// Outputs: none
// This will put a stripped bitmap (as per valvanoware) into the buffer, inverting colors present. 0x0000 is ignored
void BM_DrawBitmapInvert_16(int32_t x, int32_t y, uint32_t width, uint32_t height, uint16_t* bitmapPtr, buffer16* b)
{
  uint32_t r,c;
  for(r = 0; r<height; r++)
  {
    for(c = 0; c<width; c++)
    {
      if(*bitmapPtr != 0x0000)
      {
        BM_DrawPixel_16(x+c,y+r,~BM_GetPixel_16(x+c,y+r,b),b);
      }
      bitmapPtr++;
    }
  }
}

// ----- Fill Rectangle -----
// Inputs : x,y,w,h,color,buffer info
// Outputs: none
// This will fill a rectangle with a certain color. The x and y position count as pixels.
// Aligned to the top left corner of the matrix. Your mileage may vary depending on the device you're using to display this array
void BM_FillRect_16(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color, buffer16* b)
{
  //Borders
  int32_t lB = x;
  int32_t rB = x + w;
  int32_t tB = y + h;
  int32_t bB = y;
  for(y=bB; y <= tB; y++) {
    for(x=lB; x<= rB; x++) {
      BM_DrawPixel_16(x,y,color,b);
    }
  }
}

// ----- Draw Column -----
// Inputs : x,y,h,color,buffer info
// Outputs: none
// This will draw a line from (x,y) to (x,y+h)
// Intended to be a fast way to draw non diagonals
void BM_DrawColumn_16(int32_t x, int32_t y, int32_t h, uint16_t color, buffer16* b)
{
    for(uint32_t i = 0; i<h; i++)
    {
        BM_DrawPixel_16(x,y+i,color,b);
    }
}
// ----- Draw Row -----
// Inputs : x,y,w,color,buffer info
// Outputs: none
// This will draw a line from (x,y) to (x+w,y)
// Intended to be a fast way to draw non diagonals
void BM_DrawRow_16(int32_t x, int32_t y, int32_t w, uint16_t color, buffer16* b)
{
    for(uint32_t i = 0; i<w; i++)
    {
        BM_DrawPixel_16(x+i,y,color,b);
    }
}


// ----- Draw Line -----
// Inputs : x1,y1,x2,y2,color,buffer info
// Outputs: none
// This will draw a line from (x1,y1) to (x2,y2) with the color using Bresnehem's algorithm
// This is taken from the Nokia5110_Functions.h
void BM_DrawLine_16(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color, buffer16* b)
{
  int32_t dx = x1-x0;
  int32_t dy = y1-y0;
  int32_t stepx, stepy;
  int32_t fraction;
    if (dy < 0)
    {
      dy = -dy;
      stepy = -1;
    }
    else
      stepy = 1;

    if (dx < 0)
    {
      dx = -dx;
      stepx = -1;
    }
    else
    {
      stepx = 1;
    }
    dy <<= 1; // dy is now 2*dy
    dx <<= 1; // dx is now 2*dx
    BM_DrawPixel_16(x0,y0,color,b); // Draw the first pixel.

    if (dx > dy) 
    {
      fraction = dy - (dx >> 1);
      while (x0 != x1)
      {
        if (fraction >= 0)
        {
          y0 += stepy;
          fraction -= dx;
        }
        x0 += stepx;
        fraction += dy;
        BM_DrawPixel_16(x0,y0,color,b);
      }
    }
    else
    {
      fraction = dx - (dy >> 1);
      while (y0 != y1)
      {
        if (fraction >= 0)
        {
          x0 += stepx;
          fraction -= dy;
        }
        y0 += stepy;
        fraction += dx;
        BM_DrawPixel_16(x0,y0,color,b);
      }
    }
}

// ----- Draw V Line -----
// Inputs : x,y0,y1,color,buffer info
// Outputs: none
// Quicker drawline: provide an x, and y and an end y for it
void BM_DrawVLine_16(int32_t x, int32_t y0, int32_t y1, uint16_t color, buffer16* bufferPtr)
{
  if(y1 > y0)
  {
    int32_t dY = y1-y0;
    for(uint32_t i = 0; i<dY; i++)
    {
      BM_DrawPixel(x,(y0+i),color,bufferPtr);
    }
  }
  else
  {
    int32_t dY = y0-y1;
    for(uint32_t i = 0; i<dY; i++)
    {
      BM_DrawPixel(x,(y0-i),color,bufferPtr);
    }
  }
}
void BM_DrawHLine_16(int32_t x0, int32_t y, int32_t x1, uint16_t color, buffer16* bufferPtr)
{
  if(x1 > x0)
  {
    int32_t dX = x1-x0;
    for(uint32_t i = 0; i<dX; i++)
    {
      BM_DrawPixel((x0-i),y,color,bufferPtr);
    }
  }
  else
  {
    int32_t dX = x0-x1;
    for(uint32_t i = 0; i<dX; i++)
    {
      BM_DrawPixel((x0-i),y,color,bufferPtr);
    }
  }
}


// ----- Draw Line Thick -----
// Inputs : x1,y1,x2,y2,color,thickness,buffer info
// Outputs: none
// Draw a thick line
void BM_DrawLineThick_16(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color, uint8_t thickness, buffer16* b)
{
  uint8_t i = 0;
  //BRILLIANT IDEA RYAN!!!
  //These fill in the voids at the beginning and end of the line
  BM_FillRect_16(x0-(thickness>>1),y0-(thickness>>1),thickness,thickness,color,b);
  BM_FillRect_16(x1-(thickness>>1),y1-(thickness>>1),thickness,thickness,color,b);
  BM_DrawLine_16(x0,y0,x1,y1,color,b);
  int32_t dx = x1-x0;
  int32_t dy = y1-y0;
  if(dx < 0){dx*=-1;}
  if(dy < 0){dy*=-1;}
  if(dx>dy)
  {
    for(i=0;i<thickness;i++)
    {
      BM_DrawLine_16(x0,y0-i,x1,y1-i,color,b);
      BM_DrawLine_16(x0,y0+i,x1,y1+i,color,b);
    }
  }
  else
  {
    for(i=0;i<thickness;i++)
    {
      BM_DrawLine_16(x0-i,y0,x1-i,y1,color,b);
      BM_DrawLine_16(x0+i,y0,x1+i,y1,color,b);
    }
  }
}
// ----- Draw Triangle-----
// Inputs : x1,y1,x2,y2,x3,y3,color,buffer info
// Outputs: none
// Draws a triangle given its points
void BM_DrawTriangle_16(int32_t p1x, int32_t p1y, int32_t p2x, int32_t p2y, int32_t p3x, int32_t p3y,uint16_t color, buffer16* buffer)
{
  BM_DrawLine_16(p1x,p1y,p2x,p2y,color,buffer);
  BM_DrawLine_16(p1x,p1y,p3x,p3y,color,buffer);
  BM_DrawLine_16(p3x,p3y,p2x,p2y,color,buffer);
}

/*
#define RED_MASK = 0xF800;
#define GREEN_MASK = 0x07E0;
#define BLUE_MASK = 0x001F;
*/
// ----- Color Darken -----
// Inputs : 565 16bit color, number of times to brighten
// Outputs: None
// This will darken a color given the number of times
uint16_t BM_ColorDarken_16(uint16_t color, uint8_t times)
{
  uint16_t output = 0;
  uint16_t temp = 0;
  for(;times >0;times--)
  {
    //output |= ((((color & RED_MASK) >> 11)--) << 11); //get red values
    temp = (color & 0xF800) >> 11;  //get red values
    if(temp)  //if red is still above zero, subtract
    {
      //output |= ((temp--)<<11);
      temp--;
      output |= temp << 11;
    }
    temp = (color & 0x07E0) >> 5; //get green values
    if(temp>1)  //if green is still above zero, subtract
    {
      //output |= ((temp--)<<5);
      temp -=2;
      output |= temp << 5;
    }
    else
    {
      temp = 0;
      output |= temp << 5;
    }
    temp = (color & 0x001F);  //get blue values
    if(temp)  //if blue is still above zero, subtract
    {
      //output |= ((temp--));
      temp--;
      output |= temp;
    }
  }
  return output;
}

// ----- Color Brighten -----
// Inputs : 565 16bit color, number of times to brighten
// Outputs: None
// This will brighten a color given the number of times
uint16_t BM_ColorBrighten_16(uint16_t color, uint8_t times)
{
  uint16_t output = 0;
  uint16_t temp = 0;
  for(;times >0;times--)
  {
    //output |= ((((color & RED_MASK) >> 11)--) << 11); //get red values
    temp = (color & 0xF800) >> 11;  //get red values
    if(temp < 255)  //if red is still above zero, subtract
    {
      //output |= ((temp--)<<11);
      temp++;
      output |= temp << 11;
    }
    temp = (color & 0x07E0) >> 5; //get green values
    if(temp < 254)  //if green is still above zero, subtract
    {
      //output |= ((temp--)<<5);
      temp += 2;
      output |= temp << 5;
    }
    else
    {
      temp = 255;
      output |= temp << 5;
    }
    temp = (color & 0x001F);  //get blue values
    if(temp < 255)  //if blue is still above zero, subtract
    {
      //output |= ((temp--));
      temp++;
      output |= temp;
    }
  }
  return output;
}

// ----- Clear Buffer -----
// Inputs : Buffer info
// Outputs: None
// This will call a fill rect that will fill everything with color 0x0000
void BM_ClearBuffer_16(buffer16* b)
{
  //I have no idea what 0xF88F is as a color. There are no calculators to convert a 16 bit color hex code bak to RGB values
  BM_FillRect_16(0,0,b->width,b->height,0x0000,b);
}

// ---- Scale Bitmap -----
// Inputs : x,y,w,h,scale,buffer,pointer to bitmap
// Outputs: none
// x and y are locations on the buffer on where to draw final product
// width, height are dimensions of bitmap to be scaled
// scaleX and scaleY are factors to scale by
// bitmapPtr is a pointer to source bitmap
// b is destination buffer
// Will scale a bitmap, putting it into the buffer at a certain scale
void BM_ScaleBitmap_16(	int32_t x,						int32_t y, 
												uint32_t width,			 	uint32_t height, 
												ufixed32_3 scaleX,	 	ufixed32_3 scaleY, 
												uint16_t* bitmapPtr, 	buffer16* b){
													
  uint32_t destWidth = (width * scaleX) / 1000;
  uint32_t destHeight = (height * scaleY) / 1000;
  
  uint32_t sourceX, sourceY;                          //defines what pixel in source bitmap to look at
  for(uint32_t i = 0;i < destHeight; i++){
    for(uint32_t j = 0; j < destWidth; j++){
      
      sourceX = j * 1000 / scaleX;
      sourceY = i * 1000 / scaleY;
      
      uint16_t pixel = bitmapPtr[sourceY * width      //get row
                                  + sourceX];         //get column
      BM_DrawPixel_16(j + x, i + y, pixel, b);      //draw pixel if not transparent
    }
  }
}

// ---- Scale Bitmap -----
// Inputs : x,y,w,h,scale,buffer,pointer to bitmap
// Outputs: none
// Will scale a bitmap, putting it into the buffer at a certain scale. Takes 0x0000 as a transparency
void BM_ScaleBitmapOver_16( int32_t x,            int32_t y, 
                            uint32_t width,       uint32_t height,
                            ufixed32_3 scaleX,    ufixed32_3 scaleY, 
                            uint16_t* bitmapPtr,  buffer16* b){
  scaleX = scaleX>10000?10000:scaleX;
  scaleY = scaleY>10000?10000:scaleY;
  uint32_t destWidth = (width * scaleX) / 1000;
  uint32_t destHeight = (height * scaleY) / 1000;
  
  uint32_t sourceX, sourceY;                          //defines what pixel in source bitmap to look at
  for(uint32_t i = 0;i < destHeight; i++){
    for(uint32_t j = 0; j < destWidth; j++){
      
      sourceX = j * 1000 / scaleX;
      sourceY = i * 1000 / scaleY;
      
      uint16_t pixel = bitmapPtr[sourceY * width      //get row
                                  + sourceX];         //get column
      if(pixel != 0){
        BM_DrawPixel_16(j + x, i + y, pixel, b);      //draw pixel if not transparent
      }
    }
  }
}

// ---- Draw Sprite -----
// Inputs : x,y,w,h,scale,buffer,pointer to sprite
// Outputs: none
// x and y are locations on the buffer on where to draw final product
// Will scale a bitmap, putting it into the buffer at a certain scale
void BM_DrawSprite_16(int32_t x, int32_t y, sprite16* spritePtr, buffer16* bufferPtr)
{
  BM_DrawBitmap_16(x - spritePtr->pivotX, y - spritePtr->pivotY, spritePtr->width, spritePtr->height, spritePtr->bitmap, bufferPtr);
}

// ---- Draw Sprite Over -----
// Inputs : x,y,w,h,scale,buffer,pointer to sprite
// Outputs: none
// x and y are locations on the buffer on where to draw final product
// Will scale a bitmap, putting it into the buffer at a certain scale
void BM_DrawSpriteOver_16(int32_t x, int32_t y, sprite16* spritePtr, buffer16* bufferPtr)
{
  BM_DrawBitmapOver_16(x - spritePtr->pivotX, y - spritePtr->pivotY, spritePtr->width, spritePtr->height, spritePtr->bitmap, bufferPtr);
}


// ---- Scale Sprite -----
// Inputs : x,y,w,h,scale,buffer,pointer to sprite
// Outputs: none
// x and y are locations on the buffer on where to draw final product
// Will scale a bitmap, putting it into the buffer at a certain scale
void BM_ScaleSprite_16(int32_t x, int32_t y, ufixed32_3 scaleX, ufixed32_3 scaleY, sprite16* spritePtr, buffer16* b){
	BM_ScaleBitmap_16(x - scaleX * spritePtr->pivotX / 1000, y - scaleY * spritePtr->pivotY / 1000, spritePtr->width, spritePtr->height, scaleX, scaleY, spritePtr->bitmap, b);
}

// ---- Scale Sprite Over -----
// Inputs : x,y,w,h,scale,buffer,pointer to sprite
// Outputs: none
// Will scale a bitmap, putting it into the buffer at a certain scale. Takes 0x0000 as a transparency
void BM_ScaleSpriteOver_16(int32_t x, int32_t y, ufixed32_3 scaleX, ufixed32_3 scaleY, sprite16* spritePtr, buffer16* b){
	BM_ScaleBitmapOver_16(x - scaleX * spritePtr->pivotX / 1000, y - scaleY * spritePtr->pivotY / 1000, spritePtr->width, spritePtr->height, scaleX, scaleY, spritePtr->bitmap, b);
}
