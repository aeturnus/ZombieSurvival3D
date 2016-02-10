#include <stdint.h>
#include "BrandonFIFO.h"

/*
#ifndef FIFO_SUCCESS
#define FIFO_SUCCESS 0
#define FIFO_EMPTY 255
#define FIFO_FULL 1
#endif
*/

#define FIFO_WAIT() (while(fifo->status == FIFO_EMPTY))

void initFIFO8(fifo8* fifo, uint8_t* buffer, uint32_t size)
{
  fifo->eI = 0;
  fifo->dI = 0;
  fifo->size = size;
  fifo->status = FIFO_EMPTY;
  fifo->queue = buffer;
}

uint8_t enqueue8(fifo8* fifo, uint8_t data)
{
  //If the next index enqueue points to is the dequeue index, it's full
  //(eI++)%size returns 0 if it equals size (and thus is out of bounds)
  if(fifo->status != FIFO_FULL)
  {
    fifo->queue[fifo->eI] = data;
    fifo->eI = (++(fifo->eI))%(fifo->size);
    if(fifo->eI == fifo->dI)
    {
      fifo->status = FIFO_FULL;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
    return FIFO_SUCCESS;
  }
  return FIFO_FULL;
}

uint8_t dequeue8(fifo8* fifo, uint8_t* dataPtr)
{
  if(fifo->status != FIFO_EMPTY)
  {
    *dataPtr = (fifo->queue)[fifo->dI];
    fifo->dI = (++(fifo->dI)%(fifo->size));
    if(fifo->dI == fifo->eI)
    {
      fifo->status = FIFO_EMPTY;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
  }
  else
  {
    return FIFO_EMPTY;
  }
  return FIFO_SUCCESS;
}

void dequeue2Bytes(fifo8* fifo, uint16_t* dataPtr)
{
  uint8_t data = 0;
  uint16_t output = 0;
  //FIFO_WAIT();
  while(fifo->status == FIFO_EMPTY);
  dequeue8(fifo,&data);
  output |= (data<<0);
  while(fifo->status == FIFO_EMPTY);
  dequeue8(fifo,&data);
  output |= (data<<8);
  *dataPtr = output;
}

void dequeue4Bytes(fifo8* fifo, uint32_t* dataPtr)
{
  uint8_t data = 0;
  uint32_t output = 0;
  while(fifo->status == FIFO_EMPTY);
  dequeue8(fifo,&data);
  output |= (data<<0);
  while(fifo->status == FIFO_EMPTY);
  dequeue8(fifo,&data);
  output |= (data<<8);
  while(fifo->status == FIFO_EMPTY);
  dequeue8(fifo,&data);
  output |= (data<<16);
  while(fifo->status == FIFO_EMPTY);
  dequeue8(fifo,&data);
  output |= (data<<24);
  *dataPtr = output;
}

void initFIFO16(fifo16* fifo, uint16_t* buffer, uint32_t size)
{
  fifo->eI = 0;
  fifo->dI = 0;
  fifo->size = size;
  fifo->status = FIFO_EMPTY;
  fifo->queue = buffer;
}

uint8_t enqueue16(fifo16* fifo, uint16_t data)
{
  //If the next index enqueue points to is the dequeue index, it's full
  //(eI++)%size returns 0 if it equals size (and thus is out of bounds)
  if(fifo->status != FIFO_FULL)
  {
    fifo->queue[fifo->eI] = data;
    fifo->eI = (++(fifo->eI))%(fifo->size);
    if(fifo->eI == fifo->dI)
    {
      fifo->status = FIFO_FULL;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
    return FIFO_SUCCESS;
  }
  return FIFO_FULL;
}

uint8_t dequeue16(fifo16* fifo, uint16_t* dataPtr)
{
  if(fifo->status != FIFO_EMPTY)
  {
    *dataPtr = (fifo->queue)[fifo->dI];
    fifo->dI = (++(fifo->dI)%(fifo->size));
    if(fifo->dI == fifo->eI)
    {
      fifo->status = FIFO_EMPTY;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
    
  }
  else
  {
    return FIFO_EMPTY;
  }
  return FIFO_SUCCESS;
}


void initFIFO32(fifo32* fifo, uint32_t* buffer, uint32_t size)
{
  fifo->eI = 0;
  fifo->dI = 0;
  fifo->size = size;
  fifo->status = FIFO_EMPTY;
  fifo->queue = buffer;
}

uint8_t enqueue32(fifo32* fifo, uint32_t data)
{
  //If the next index enqueue points to is the dequeue index, it's full
  //(eI++)%size returns 0 if it equals size (and thus is out of bounds)
  if(fifo->status != FIFO_FULL)
  {
    fifo->queue[fifo->eI] = data;
    fifo->eI = (++(fifo->eI))%(fifo->size);
    if(fifo->eI == fifo->dI)
    {
      fifo->status = FIFO_FULL;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
    return FIFO_SUCCESS;
  }
  return FIFO_FULL;
}

uint8_t dequeue32(fifo32* fifo, uint32_t* dataPtr)
{
  if(fifo->status != FIFO_EMPTY)
  {
    *dataPtr = (fifo->queue)[fifo->dI];
    fifo->dI = (++(fifo->dI)%(fifo->size));
    if(fifo->dI == fifo->eI)
    {
      fifo->status = FIFO_EMPTY;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
  }
  else
  {
    return FIFO_EMPTY;
  }
  return FIFO_SUCCESS;
}


void initFIFO64(fifo64* fifo, uint64_t* buffer, uint32_t size)
{
  fifo->eI = 0;
  fifo->dI = 0;
  fifo->size = size;
  fifo->status = FIFO_EMPTY;
  fifo->queue = buffer;
}

uint8_t enqueue64(fifo64* fifo, uint64_t data)
{
  //If the next index enqueue points to is the dequeue index, it's full
  //(eI++)%size returns 0 if it equals size (and thus is out of bounds)
  if(fifo->status != FIFO_FULL)
  {
    fifo->queue[fifo->eI] = data;
    fifo->eI = (++(fifo->eI))%(fifo->size);
    if(fifo->eI == fifo->dI)
    {
      fifo->status = FIFO_FULL;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
    return FIFO_SUCCESS;
  }
  return FIFO_FULL;
}

uint8_t dequeue64(fifo64* fifo, uint64_t* dataPtr)
{
  if(fifo->status != FIFO_EMPTY)
  {
    *dataPtr = (fifo->queue)[fifo->dI];
    fifo->dI = (++(fifo->dI)%(fifo->size));
    if(fifo->dI == fifo->eI)
    {
      fifo->status = FIFO_EMPTY;
    }
    else
    {
      fifo->status = FIFO_SUCCESS;
    }
  }
  else
  {
    return FIFO_EMPTY;
  }
  return FIFO_SUCCESS;
}

