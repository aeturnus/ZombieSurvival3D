//Brandon Nguyen

#include <stdint.h>
#include "dac.h"
#include "../tm4c123gh6pm.h"
#include "Sound.h"

#define PF_EOR(bit) GPIO_PORTF_DATA_R ^= (0x01<<bit)


sound* soundArray;

uint16_t soundIndex = 0;
sound toPlay;

uint8_t active = 0;


//---- Initialize Sound ----
void Sound_Init(sound* sounds){
  soundArray = sounds;
  soundIndex = 0;
  active = 0;
	//toPlay = &InitSound;
	//Timer0_Init(&wavPlayer, 7256);
}


//---- Sound_Play ----
// Outputs single DAC value from a WAV array
void Sound_Play(){
	active = 1;
}
//---- Sound_Stop ----
// Outputs single DAC value from a WAV array
void Sound_Stop(){
	active = 0;
}

//---- Sets Sound to be played on next Timer0 Interrupt. Plays it
// See header for possible sounds to play
void Sound_Load(uint8_t index){
	toPlay = soundArray[index];
	soundIndex = 0;
}

//----- Sound_Out -----
//Outputs
void Sound_Out(void)
{
  if(soundIndex < toPlay.length && active){
    soundIndex++;
		DAC_Out(toPlay.soundPtr[soundIndex]);
	}
  else
  {
    active = 0;
  }
}