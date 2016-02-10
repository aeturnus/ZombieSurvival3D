//Brandon Nguyen

#ifndef __SOUND_H__
#define __SOUND_H__
#include <stdint.h>


typedef struct S_Sound{
	uint32_t length;
	uint8_t* soundPtr;
} sound;

//---- Initialize Sound ----
void Sound_Init(sound* sounds);
//---- Sound_Play ----
// Outputs single DAC value from a WAV array
void Sound_Play(void);
void Sound_Stop(void);
//---- Sets Sound to be played on next Timer0 Interrupt
// See header for possible sounds to play
void Sound_Load(uint8_t songIndex);
//----- Sound_Out -----
//Outputs
void Sound_Out(void);

#endif
