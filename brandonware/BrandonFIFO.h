#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>


//return codes
#define FIFO_SUCCESS 0
#define FIFO_EMPTY 255
#define FIFO_FULL 1

#define waitForFIFO(a) while(a.status == FIFO_EMPTY){}    //a is fifo struct

typedef struct FIFO_8
{
	uint32_t eI; //enqueue index
	uint32_t dI; //dequeue index
	uint32_t size; //how large the queue is
	uint32_t status; //0:normal,-1:empty,1:full
	uint8_t* queue;	//queue pointer

}fifo8;

typedef struct FIFO_16
{
	uint32_t eI; //enqueue index
	uint32_t dI; //dequeue index
	uint32_t size; //how large the queue is
	uint32_t status; //0:normal,-1:empty,1:full
	uint16_t* queue;	//queue pointer

}fifo16;

typedef struct FIFO_32
{
	uint32_t eI; //enqueue index
	uint32_t dI; //dequeue index
	uint32_t size; //how large the queue is
	uint32_t status; //0:normal,-1:empty,1:full
	uint32_t* queue;	//queue pointer

}fifo32;


typedef struct FIFO_64
{
	uint32_t eI; //enqueue index
	uint32_t dI; //dequeue index
	uint32_t size; //how large the queue is
	uint32_t status; //0:normal,-1:empty,1:full
	uint64_t* queue;	//queue pointer

}fifo64;

void initFIFO8(fifo8* fifo, uint8_t* buffer, uint32_t size);
uint8_t enqueue8(fifo8* fifo, uint8_t data);
uint8_t dequeue8(fifo8* fifo, uint8_t* dataPtr);
void dequeue2Bytes(fifo8* fifo, uint16_t* dataPtr);
void dequeue4Bytes(fifo8* fifo, uint32_t* dataPtr);

void initFIFO16(fifo16* fifo, uint16_t* buffer, uint32_t size);
uint8_t enqueue16(fifo16* fifo, uint16_t data);
uint8_t dequeue16(fifo16* fifo, uint16_t* dataPtr);

void initFIFO32(fifo32* fifo, uint32_t* buffer, uint32_t size);
uint8_t enqueue32(fifo32* fifo, uint32_t data);
uint8_t dequeue32(fifo32* fifo, uint32_t* dataPtr);

void initFIFO64(fifo64* fifo, uint64_t* buffer, uint32_t size);
uint8_t enqueue64(fifo64* fifo, uint64_t data);
uint8_t dequeue64(fifo64* fifo, uint64_t* dataPtr);

#endif
