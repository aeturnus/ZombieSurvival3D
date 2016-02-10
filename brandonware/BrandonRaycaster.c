#include <stdint.h>
#include "BrandonTypes.h"
#include "BrandonMath.h"
#include "BrandonRaycaster.h"

void RC_InitWorld(world* worldPtr,uint8_t* worldArray,uint32_t width,uint32_t height)
{
	worldPtr->worldArray = worldArray;
	worldPtr->width = width;
	worldPtr->height = height;
}

uint8_t RC_GetTile(world* worldPtr,uint32_t x, uint32_t y)
{
    ///* It's on the designer to stay in the bounds
    if(x > worldPtr->width || y > worldPtr->height)
    {
        return 0x00;
    }
    //*/
    return worldPtr->worldArray[x + y * worldPtr->width];
}

void RC_UpdateCamera(camera* cameraPtr)
{
	//cameraPtr->direction = normalizeAngleFix(cameraPtr->direction);
  normalizeAngleFixPtr(&(cameraPtr->direction));
	if(cameraPtr->FOVchanged)
	{
		fixed32_3 columns = toFixed_3(cameraPtr->columns);
		cameraPtr->focal = div32_3(columns/2,tangentFix((cameraPtr->FOV)/2));			//Calculate the new focal length
		cameraPtr->dAngle = div32_3(cameraPtr->FOV,toFixed_3(cameraPtr->columns));	//Calculate the change in angle
		cameraPtr->FOVchanged = 0;	//Unset the flag
	}
}

void RC_InitCamera(camera* cameraPtr,fixed32_3 x, fixed32_3 y, fixed32_3 rot, fixed32_3 FOV, uint32_t cols, uint32_t rows, fixed32_3 range, ray* rayArray)
{
  cameraPtr->pos.x = x;
  cameraPtr->pos.y = y;
  cameraPtr->pos.z = 0;
  cameraPtr->direction = rot;
  cameraPtr->FOV = FOV;
  cameraPtr->columns = cols;
  cameraPtr->rows = rows;
  cameraPtr->range = range;
  cameraPtr->rayArray = rayArray;
  cameraPtr->FOVchanged = 1;
  RC_UpdateCamera(cameraPtr);
}

void RC_CastRays(camera* cameraPtr, world* worldPtr)
{
	//OMG IT'S REALLY HAPPENING
	//Local vars to reduce access time via one less indirection
	fixed32_3 FOV = cameraPtr->FOV;
	fixed32_3 cAngle = cameraPtr->direction - FOV/2;   //Current ray angle
	fixed32_3 dAngle = cameraPtr->dAngle;              //The change in angle. Reduce an indirection with local var
	uint16_t cols = cameraPtr->columns;                //# of colums. Reduce indirection


    uint16_t i;
	for(i = 0; i<cols; i++)
	{

		//Check for vertical line intersections
    RC_CastRay(cameraPtr,worldPtr,cAngle,i);
    //RC_CastRay(cameraPtr,worldPtr,90-(FOV/2+dAngle*i),i);
		//Fisheye correction:
		//
		/*
		if(i < cols/2)
		{
			cameraPtr->rayArray[i].distance = mul32_3(cameraPtr->rayArray[i].distance,cosineFix(FOV/2 - i * dAngle));
		}
		else
		{
			cameraPtr->rayArray[i].distance = mul32_3(cameraPtr->rayArray[i].distance,cosineFix((i-cols/2) * (dAngle)));
		}
    */
		cameraPtr->rayArray[i].distance = mul32_3(cameraPtr->rayArray[i].distance,sineFix(FOV/2 + dAngle*i));
		//
		//
		cAngle += dAngle;
	}
}


void RC_CastRay(camera* cameraPtr,world* worldPtr, fixed32_3 angle,uint32_t index)
{

    /* How my walldata works:
     * The most significant nibble identifies what type of tile it is
     * 0000 xxxx is an empty tile. This is meant as a floor. The rest can be data as you see fit
     *
     *
     */
    #define RAYCASTER 0
    normalizeAngleFixPtr(&angle);
    fixed32_3 distance=0;
    uint8_t wallData=0;
    uint8_t wallMeta=0;
    ufixed16_3 wallOffset=0;
  
  
    #if (RAYCASTER==0)
    //Legit raycaster
    fixed32_3 x,y,yA,xA,dY,dX,xD,yD,xX,yX,xY,yY;
    uint32_t counter = 0;
    uint32_t range = cameraPtr->range /1000;
    uint8_t neverHit = 0;
    x = cameraPtr->pos.x;
    y = cameraPtr->pos.y;
    //Calculate the x-intercepts
    if(angle<=180000)
    {
      yA = ((y/1000)) * (1000) + 1000;
      dY = 1000;
    }
    else
    {
      yA = ((y)/1000) * (1000) - 1;
      dY = -1000;
    }
    xA = x + div32_3((y - yA),tangentFix(angle));
    dX = div32_3(1000,tangentFix(angle));
    
    //xD = xA - x;  //add initial movement
    //yD = yA - y;  //add initial movement
    xX = xA;       //move ray pos up
    yX = yA;       //
    wallData = RC_GetTile(worldPtr,xX/1000,yX/1000);
    while((wallData&0x10) == 0 && counter < range && neverHit == 0)
    {
      xX += dX;
      yX += dY;
      if(xX >= 0 && (xX/1000) < worldPtr->width && yX >= 0 && (yX/1000) < worldPtr->height)
      {
        wallData = RC_GetTile(worldPtr,xX/1000,yX/1000);
      }
      else
      {
        neverHit = 1;
      }
      counter++;
    }
    if(!neverHit)
    {
      xD = div32_3(abs(x - xX),abs(cosineFix(angle)));  //X hit distance
    }
    else
    {
      xD = 0x7FFFFFFF;
    }
    
    //Calculate the y-intercepts
    if((angle<90000 && angle>0)||(angle>270000&&angle<360000))
    {
      xA = ((x/1000) * (1000)) + 1000;
      dX = 1000;
    }
    else
    {
      xA = ((x/1000) * (1000)) - 1 ;
      dX = -1000;
    }
    yA = y + mul32_3((x - xA),tangentFix(angle));
    dY = mul32_3(1000,tangentFix(angle));
    
   // xD = xA - x;  //add initial movement
    //yD = yA - y;  //add initial movement
    xY = xA;       //move ray pos up
    yY = yA;       //
    wallData = RC_GetTile(worldPtr,xY/1000,yY/1000);
    counter = 0;
    neverHit = 0;
    while((wallData&0x10) == 0 && counter < range && neverHit == 0)
    {
      xY += dX;
      yY += dY;
      if(xY >= 0 && (xY/1000) < worldPtr->width && yY >= 0 && (yY/1000) < worldPtr->height)
      {
        wallData = RC_GetTile(worldPtr,xY/1000,yY/1000);
      }
      else
      {
        neverHit = 1;
      }
      counter++;
    }
    if(!neverHit)
    {
      yD = div32_3(abs(x - xY),abs(cosineFix(angle)));  //Y hit distance
    }
    else
    {
      yD = 0x7FFFFFFF;
    }
    
    if(xD < yD)
    {
      //X hit
      distance = xD;
      wallData = RC_GetTile(worldPtr,xX/1000,yX/1000);
      wallMeta = X_HIT;
      wallOffset = xX%1000;
    }
    else if ( yD <= xD)
    {
       distance = yD;
      wallData = RC_GetTile(worldPtr,xY/1000,yY/1000);
      wallMeta = Y_HIT;
      wallOffset = yY%1000;
    }
    else
    {
      distance = 0x7FFFFFFF;
    }
    
    
    #else
    
    //I present: the shittiest distance finder ever. Plods along at a constant dR
    #define DISTANCE_CHANGE 50 //dR constant is .100
    fixed32_3 dX = mul32_3(DISTANCE_CHANGE,cosineFix(angle));    //get the change in x
    fixed32_3 dY = mul32_3(DISTANCE_CHANGE,sineFix(angle));      //get the change in y
    fixed32_3 x = cameraPtr->pos.x;                    //start position x
    fixed32_3 y = cameraPtr->pos.y;                    //start position y

    distance = 0;
    wallData = RC_GetTile(worldPtr,x/1000,y/1000);
    wallMeta = 0x00;
    wallOffset = 0;
    //trace the ray
    while((wallData&0x10) == 0)   //If even, ray passes through
    {
        //Update ray position
        x += dX;
        y += dY;
        distance += DISTANCE_CHANGE;
        wallData = RC_GetTile(worldPtr,x/1000,y/1000);
				if(distance > cameraPtr->range)
				{
					break;
				}
    }

    //establish metadata
    //wallMeta |= x%1000 < 500? WEST_HIT : EAST_HIT;      //If the fractional part of x is < .5, it's a west hit
    //wallMeta |= y%1000 < 500? SOUTH_HIT : NORTH_HIT;    //If the fractional part of y is < .5, it's a south hit
    //wallOffset = x%1000;
    uint16_t modX = x%1000;
    uint16_t modY = y%1000;
    /*
    if((modX)< 0+(DISTANCE_CHANGE+20) || (modX) > 1000-(DISTANCE_CHANGE+20))
    {
      if(modY > modX)
      {
        wallMeta |= X_HIT;
        wallOffset = modX;
      }
      else
      {
        wallMeta |= X_HIT;
        wallOffset = modY;
      }
    }
    else
    {
      if(modX > modY)
      {
        wallMeta |= Y_HIT;
        wallOffset = modY;
      }
      else
      {
        wallMeta |= Y_HIT;
        wallOffset = modX;
      }
    }
    */
    if((modX)< 0+(DISTANCE_CHANGE+0) || (modX) > 1000-(DISTANCE_CHANGE+0))
    {
      wallMeta |= X_HIT;
      wallOffset = modX;
    }
    else
    {
      wallMeta |= Y_HIT;
      wallOffset = modY;
    }
    
    #endif

    cameraPtr->rayArray[index].distance = distance;
    cameraPtr->rayArray[index].wallData = wallData;
    cameraPtr->rayArray[index].wallMeta = wallMeta;
    cameraPtr->rayArray[index].wallOffset = wallOffset;

/*
    //////The legit way
    fixed32_3 dX;
	fixed32_3 dY;
	//Check for horizontal line intersections
    dX = div32_3(1000,tangentFix(cAngle));
*/
}
