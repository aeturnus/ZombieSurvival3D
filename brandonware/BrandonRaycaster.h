#ifndef BRANDONRAYCASTER_H
#define BRANDONRAYCASTER_H

#include <stdint.h>
#include "BrandonTypes.h"
#include "BrandonMath.h"

//Defines for tile types
//See through tiles are even
#define FLOOR_T         0x00 //Standard floor
#define FLOORC_T        0x10 //Walk throughable wall
#define WALLC_T         0x20 //See throughable wall
#define WALL_T          0x30 //Standard wall
#define DOORC_T         0x40 //See through door
#define DOOR_T          0x50 //Door

//Defines for tile metadata
#define WEST_HIT        0x0 //
#define SOUTH_HIT       0x1 //
#define NORTH_HIT       0x2 //
#define EAST_HIT        0x4 //
#define X_HIT             0x1
#define Y_HIT             0x2


typedef struct RC_Ray
{
    fixed32_3 distance;     //Distance data for each column
    uint8_t wallData;       //What tile type it had run into
    uint8_t wallMeta;       //Any additional metadata that it has e.g. N/E/S/W data shading
    ufixed16_3 wallOffset;  //Offset from corner for texture mapping
} ray;

typedef struct RC_World
{
	uint8_t* worldArray;
	uint32_t width;
	uint32_t height;
} world;

typedef struct RC_Camera
{
	//Entity types have these pieces of data
	vector pos;				//Position vector data
	vector vel;				//
	vector acc;				//
	fixed32_3 direction;			//Degrees
	//
	uint16_t columns;				//Number of columns to draw
	uint16_t rows;					//How many rows?
	uint8_t FOVchanged;				//Flag for a change in FOV, check to see if focal and dAngle need changing
	fixed32_3 FOV;					//Field of view °, the master quantity
	fixed32_3 focal;				//Focal length, from columns/tan(FOV/2)
	fixed32_3 dAngle;				//Angle change for each column: FOV/columns
	fixed32_3 range;				//Max distance
	ray* rayArray;
	

    /*
	fixed32_3* distances;		//Distance data for each column
	uint8_t* wallData;			//What tile type it had run into
	uint8_t* wallMeta;				//Any additional metadata that it has e.g. N/E/S/W data shading
	*/
} camera;

void RC_InitWorld(world* worldPtr,uint8_t* worldArray,uint32_t width,uint32_t height);
void RC_InitCamera(camera* cameraPtr,fixed32_3 x, fixed32_3 y, fixed32_3 rot, fixed32_3 FOV, uint32_t cols, uint32_t rows, fixed32_3 range, ray* rayArray);
uint8_t RC_GetTile(world* worldPtr,uint32_t x, uint32_t y);
void RC_UpdateCamera(camera* cameraPtr);
void RC_CastRays(camera* cameraPtr, world* worldPtr);
void RC_CastRay(camera* cameraPtr,world* worldPtr, fixed32_3 angle,uint32_t index);



#endif
