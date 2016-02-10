#ifndef __BRANDONPHYSICS_H__
#define __BRANDONPHYSICS_H__

#include <stdint.h>
#include "BrandonTypes.h"
#include "BrandonMath.h"

/* This physics library falls under the assumption that movement is in the x-y plane, with the z plane being
 * out of the plane.
 * This will handle collisions between entities. If implementing this in an RC_world, you need to handle collisions
 * there
 */

//Definitions
#define MISS      0
#define COLLIDE   1


typedef struct PH_Dimension
{
  fixed32_3 width;    //x axis
  fixed32_3 height;   //y axis
  fixed32_3 depth;    //z axis
}dimension;

typedef struct PH_Entity
{
  vector pos;       //Position vector
  vector vel;       //Velocity vector
  vector acc;       //Acceleration vector
  fixed32_3 rot;    //Just where it's facing. Can be independent of moevement...or not

  dimension dim;    //Dimensions of object
}entity;



//---- Entity Act -----
// Inputs: pointer to entity, change in time
// Output: none
// Will process movement for an entity given a change in time. NOTE: you calibrate the deltaT

void PH_EntityAct(entity* entityPtr,fixed32_3 dT);

//---- IsInEntity -----
// Inputs: point, pointer to entity 2
// Output: 1 for true, 0 for not true 
// Will check for a point being inside an entity 
uint8_t PH_IsInEntity(point spot,entity* entityPtr);

//---- Check Collision -----
// Inputs: pointer to entity 1, pointer to entity 2
// Output: 1 for collision, 0 for no collision 
// Will check for a colision between two entities
uint8_t PH_CheckCollision(entity* entityPtr0,entity* entityPtr1);


#endif
