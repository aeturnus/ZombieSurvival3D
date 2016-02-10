#ifndef	__BRANDONMML_H__
#define __BRANDONMML_H__

#include <stdint.h>
#include "BrandonTypes.h"

typedef struct BMML_Track
{
	uint16_t pc;				//"program counter" of the track
	uint8_t* program;			//pointer to instructions
	uint8_t instrument;			//instrument number
	uint8_t octave;
	uint8_t tempo;
	uint8_t volume;
	uint8_t length;
	uint8_t counter;			//loop counter. can set in program. Jump will reduce loop counter by 1

	uint16_t pitch;				//Current frequency
	

	uint16_t tableIndex;    //Instrument waveform table index

	ufixed32_3 tickCount;     // How many ticks have elapsed
	ufixed32_3 tickTotal;			// Calculated by note
	ufixed32_3 tickWhole;     // How many total ticks for a whole note
	ufixed32_3 tickICount;		// Number of ticks elapsed since last index change
	ufixed32_3 tickIndex;			// How many ticks per index change
	
	uint32_t output;				// Output voltage
}track_t;

typedef struct BMML_TrackHolder
{
	uint8_t length;	//how many tracks it holds
	struct BMML_Track* tracks;	//pointer to array of BMML_Tracks
	uint32_t output;	//the total output
  uint8_t runningLength;  //How many tracks are playing at the moment
}holder_t;

typedef struct BMML_Song
{
  uint8_t trackNum;      //Number of tracks the song has. God help you if you have more than 255
  uint8_t** songTracks; //Lol pointer to pointers. Points to an array of uint8 arrays
}song_t;

typedef struct BMML_Instrument
{
  uint8_t attack[128];
  uint8_t body[128];
}instrument_t;

uint32_t BMML_CalcWhole(uint32_t busClock, uint8_t tempo, uint32_t reload);
void BMML_TrackInit(struct BMML_Track* track, uint32_t reload);
void BMML_TrackUpdate(struct BMML_Track* track, uint32_t reload);
void BMML_ParseProgram(struct BMML_Track* track, uint32_t reload);
void BMML_HolderUpdate(struct BMML_TrackHolder* holder, uint32_t reload);
void BMML_HolderInit(struct BMML_TrackHolder* holder, uint32_t reload);
void BMML_TrackLoadSong(struct BMML_Track* track, uint32_t reload, uint8_t* program);
void BMML_HolderLoadSong(struct BMML_TrackHolder* holder, struct BMML_Song* song, uint32_t reload);


#endif
