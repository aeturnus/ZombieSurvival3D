#include <stdint.h>
#include "BrandonPhysics.h"
#include "BrandonMath.h"
#include "BrandonTypes.h"

//---- Entity Act -----
// Inputs: pointer to entity, change in time
// Output: none
// Will process movement for an entity given a change in time. NOTE: you calibrate the deltaT

void PH_EntityAct(entity* entityPtr,fixed32_3 dT)
{
  
  entityPtr->pos.x += mul32_3(entityPtr->vel.x,dT);
  entityPtr->pos.y += mul32_3(entityPtr->vel.y,dT);
  entityPtr->pos.z += mul32_3(entityPtr->vel.z,dT);

  entityPtr->vel.x += mul32_3(entityPtr->acc.x,dT);
  entityPtr->vel.y += mul32_3(entityPtr->acc.y,dT);
  entityPtr->vel.z += mul32_3(entityPtr->acc.z,dT); 
}

//---- IsInEntity -----
// Inputs: point, pointer to entity 2
// Output: 1 for true, 0 for not true 
// Will check for a point being inside an entity 
uint8_t PH_IsInEntity(point spot,entity* entityPtr)
{
  vector pos = entityPtr->pos;
  fixed32_3 w = entityPtr->dim.width;
  fixed32_3 h = entityPtr->dim.height;
  fixed32_3 d = entityPtr->dim.depth;
  if(spot.x >= (pos.x - (w>>1)) && spot.x <= (pos.x + (w>>1)))
  {
    if(spot.y >= (pos.y - (h>>1)) && spot.y <= (pos.y + (h>>1)))
    {
      if(spot.z >= (pos.z - (h>>1)) && spot.z <= (pos.z + (d>>1)))
      {
        return 1;
      }
    }
  }
  return 0;
}

//---- Check Collision -----
// Inputs: pointer to entity 1, pointer to entity 2
// Output: 1 for collision, 0 for no collision 
// Will check for a colision between two entities
uint8_t PH_CheckCollision(entity* entityPtr0,entity* entityPtr1)
{
  point spot;
  vector pos = entityPtr0->pos;
  fixed32_3 w = entityPtr0->dim.width;
  fixed32_3 h = entityPtr0->dim.height;
  fixed32_3 d = entityPtr0->dim.depth;
  //Eight corner check
  spot.x = pos.x + (w>>1);
  spot.y = pos.y + (h>>1);
  spot.z = pos.z + (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x + (w>>1);
  spot.y = pos.y + (h>>1);
  spot.z = pos.z - (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x + (w>>1);
  spot.y = pos.y - (h>>1);
  spot.z = pos.z + (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x + (w>>1);
  spot.y = pos.y - (h>>1);
  spot.z = pos.z - (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x - (w>>1);
  spot.y = pos.y + (h>>1);
  spot.z = pos.z + (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x - (w>>1);
  spot.y = pos.y + (h>>1);
  spot.z = pos.z - (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x - (w>>1);
  spot.y = pos.y - (h>>1);
  spot.z = pos.z + (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}

  spot.x = pos.x - (w>>1);
  spot.y = pos.y - (h>>1);
  spot.z = pos.z - (d>>1);
  if(PH_IsInEntity(spot,entityPtr1)){return 1;}
  return 0;
}