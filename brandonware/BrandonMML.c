#include <stdint.h>
#include "BrandonMML.h"
#include "BrandonTypes.h"


/*
 * Author: Brandon Nguyen
 * The "synth" library will provide functions to generate sounds to output to the DAC
 * Multiple tracks can be supported
 * This has the waveform tables for various instruments
 * This is meant to be used with an 8-bit DAC.
 * This will parse a subset of the Music Macro Language (MML) that includes ties, dots, and jumps
 * 
 * Method of Operation:
 * The synth library will interpret a string of commands which will define behaviours. This is effectively an implementation of Music Macro Language
 * The commands will essentially work similar machine
 * Commands:
  * 
 * Example:
 * "o4v9t120|" .bmml file
 * "o\x04v\x09t\x78" use converter to produce this
 * 
 */

 /*
  * Command library
  *
  * -------------
  * 'o' - octave
  * -------------
  * Will set the current track
  * Operand range: 1-8. 4 is middle
  *
  * -------------
  * 'v' - volume
  * -------------
  * Will set the volume via amplitude coefficient
  * Operand range: 1-15
  *
  * -------------
  * 't' - tempo
  * -------------
  * Will set the tempo by bpm
  * Operand range: 1-255
  * 
  * -------------
  * 'l' - length
  * -------------
  * Will set the default note division length -> 1 is whole, 2 is half, 4 is quarter etc
  * Operand range: 1,2,4,8,12,16,24,32,64
  *
  * -------------
  * 'i' - instrument
  * -------------
  * Will set the instrument
  * Operand range: 0-255
  *
  * --------------
  * '<' - lower octave
  * --------------
  * Decrease octave by one  
  *
  * ---------------
  * '>' - raise octave
  * ---------------
  * Increase octave by one
  *
  * ---------------
  * 'j' '-' '+' - jump if counter > 0
  * ---------------
  * Will change the PC by increasing(+), decreasing(-), or setting(none) depending on the following operand
  *
  * ---------------
  * 'k' '-' '+' - set the counter to, decrease, or increase
  * ---------------
  * Will change the counter by increasing(+), decreasing(-), or setting(none) depending on the following operand
  *
  * 'a,b,c,d,e,f,g' - note; 'r' for rest, n can use index
  * -------------
  * Will play a note for the set note division at the tempo, in the set octave and volume
  * Operand 1: '-' flat, '+' sharp
  * Optional operand 2: 1,2,4,8,12,16,24,32,64

  */

#ifndef BUS_CLOCK
#define BUS_CLOCK 80000000
#endif

/* Instrument waveform tables */

//Number of elements
#define ELEMENTS 128

const uint8_t sineTable[128] =
	{	128, 134, 140, 146, 152, 158, 164, 170, 176, 182, 187, 193, 198, 203, 208, 213, 217, 222, 226, 230, 233, 236, 240, 242, 245, 247, 249, 251, 252, 253, 254, 254
		, 255, 254, 254, 253, 252, 251, 249, 247, 245, 242, 240, 236, 233, 230, 226, 222, 217, 213, 208, 203, 198, 193, 187, 182, 176, 170, 164, 158, 152, 146, 140, 134
		, 128, 121, 115, 109, 103, 97, 91, 85, 79, 73, 68, 62, 57, 52, 47, 42, 38, 33, 29, 25, 22, 19, 15, 13, 10, 8, 6, 4, 3, 2, 1, 1
		, 1, 1, 1, 2, 3, 4, 6, 8, 10, 13, 15, 19, 22, 25, 29, 33, 38, 42, 47, 52, 57, 62, 68, 73, 79, 85, 91, 97, 103, 109, 115, 121
	};
  
  
const struct BMML_Instrument sine = {
  {	128, 134, 140, 146, 152, 158, 164, 170, 176, 182, 187, 193, 198, 203, 208, 213, 217, 222, 226, 230, 233, 236, 240, 242, 245, 247, 249, 251, 252, 253, 254, 254
		, 255, 254, 254, 253, 252, 251, 249, 247, 245, 242, 240, 236, 233, 230, 226, 222, 217, 213, 208, 203, 198, 193, 187, 182, 176, 170, 164, 158, 152, 146, 140, 134
		, 128, 121, 115, 109, 103, 97, 91, 85, 79, 73, 68, 62, 57, 52, 47, 42, 38, 33, 29, 25, 22, 19, 15, 13, 10, 8, 6, 4, 3, 2, 1, 1
		, 1, 1, 1, 2, 3, 4, 6, 8, 10, 13, 15, 19, 22, 25, 29, 33, 38, 42, 47, 52, 57, 62, 68, 73, 79, 85, 91, 97, 103, 109, 115, 121
	}
  ,{	128, 134, 140, 146, 152, 158, 164, 170, 176, 182, 187, 193, 198, 203, 208, 213, 217, 222, 226, 230, 233, 236, 240, 242, 245, 247, 249, 251, 252, 253, 254, 254
		, 255, 254, 254, 253, 252, 251, 249, 247, 245, 242, 240, 236, 233, 230, 226, 222, 217, 213, 208, 203, 198, 193, 187, 182, 176, 170, 164, 158, 152, 146, 140, 134
		, 128, 121, 115, 109, 103, 97, 91, 85, 79, 73, 68, 62, 57, 52, 47, 42, 38, 33, 29, 25, 22, 19, 15, 13, 10, 8, 6, 4, 3, 2, 1, 1
		, 1, 1, 1, 2, 3, 4, 6, 8, 10, 13, 15, 19, 22, 25, 29, 33, 38, 42, 47, 52, 57, 62, 68, 73, 79, 85, 91, 97, 103, 109, 115, 121
	}
};



const uint16_t pitchTable[] = {
//   C     C#    D     D#    E     F     F#    G     G#    A     A#    B
     16,   17,   18,   19,   21,   22,   23,   25,   26,   28,   29,   31,  //O0
     33,   35,   37,   39,   41,   44,   46,   49,   52,   55,   58,   62,  //O1
     65,   69,   73,   78,   82,   87,   93,   98,  104,  110,  117,  123,  //02
    131,  139,  147,  156,  165,  175,  185,  196,  208,  220,  233,  247,  //03
    262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,  //O4
    523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,  //O5
   1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,  //O6
   2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,  //O7
   4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,  //O8
};

const uint8_t* instruments[] = {sineTable};




//key definitions

//---BMML_CalcWhole---
//Inputs: bus clock, tempo, reload value of timer
//Outputs: Number of timer interrupts for a whole note
//Calculates the number of timer interrupts for a whole note to occur
ufixed32_3 BMML_CalcWhole(uint32_t busClock, uint8_t tempo, uint32_t reload)
{
  //ufixed64_3 output = toFixed_3(( (busClock * 4 / (reload * tempo)) * 60));
  //ufixed32_3 output = udiv32_3_lhp(toFixed_3(busClock * 4),toFixed_3(reload*tempo)) * 60;
  //ufixed32_3 output = toFixed_3(busClock * 4 / (reload * tempo) * 60);
  uint64_t bus = (uint64_t)busClock;
  uint64_t temp = (uint64_t)tempo;
  uint64_t rel = (uint64_t)reload;
  ufixed64_3 bus4 = (ufixed64_3)toFixed_3(bus * 4);
  ufixed64_3 denom = (ufixed64_3)toFixed_3(temp * rel);
  ufixed64_3 buffer = udiv64_3(bus4,denom) * 60;
  ufixed32_3 output = (ufixed32_3)buffer;
  return output;
}

uint8_t blank[] = "";

//---BMML_TrackInit---
//Inputs : Pointer to BMML_Track struct
//Outputs: None
//
void BMML_TrackInit(struct BMML_Track* track, uint32_t reload)
{
  track->program = blank;
  track->pc = 0;
  track->length = 4;
  track->tempo = 120;
  track->octave = 4;
  track->volume = 7;
  track->instrument = 0;
  track->counter = 0;
  track->tickCount = 0;
  track->tickICount = 0;
  track->tickTotal = 0;
  track->tickIndex = 0;
  track->tickWhole = BMML_CalcWhole(BUS_CLOCK,track->tempo,reload); //Calculate the total amount of interrupts per whole note
}

void BMML_TrackLoadSong(struct BMML_Track* track, uint32_t reload, uint8_t* program)
{
  BMML_TrackInit(track,reload); //Reinitialize 
  track->program = program;
}

void BMML_HolderLoadSong(struct BMML_TrackHolder* holder, struct BMML_Song* song, uint32_t reload)
{
  uint8_t holderIndex,songIndex;
  holder->runningLength = 0;
  for(holderIndex = 0, songIndex = 0; holderIndex < holder->length && songIndex < song->trackNum; holderIndex++,songIndex++)
  {
    BMML_TrackLoadSong(&(holder->tracks[holderIndex]), reload, song->songTracks[songIndex]);
    (holder->runningLength)++;
  }
  for(; holderIndex < holder->length; holderIndex++)
  {
    BMML_TrackLoadSong(&(holder->tracks[holderIndex]),reload,(uint8_t*)blank);  //null the rest of the tracks
  }
}

void BMML_HolderUpdate(struct BMML_TrackHolder* holder, uint32_t reload)
{
  uint8_t length = holder->runningLength;
  uint32_t output = 0;
  for(uint8_t i = 0;i<length;i++)
  {
    BMML_TrackUpdate(&(holder->tracks[i]),reload);
    output += ((holder->tracks[i]).output); //Add each of the track's outputs 
  }
  holder->output = (output/(length)); //Divide so you don't saturate
}
void BMML_HolderInit(struct BMML_TrackHolder* holder, uint32_t reload)
{
  uint8_t length = holder->length;
  for(uint8_t i = 0;i<length;i++)
  {
    BMML_TrackInit(&(holder->tracks[i]),reload);
  }
}

//---BMML_TrackUpdate---
//Inputs : Pointer to BMML_Track struct
//Outputs: None
//
void BMML_TrackUpdate(struct BMML_Track* track, uint32_t reload)
{
  if(track->program[track->pc] == 0)
  {
    return;
  }
  //Update the track's current note
  if(track->tickCount >= track->tickTotal)
  {
    BMML_ParseProgram(track,reload);
    track->tableIndex = 0;  //reset the index
    //TODO: make a better way
    //track->tickIndex = track->tickWhole * track->tempo /(4 * 60 * (track->pitch) * ELEMENTS); //calculate the number of (ticks) per index increment
    //track->tickIndex = BUS_CLOCK / (track->pitch * reload * ELEMENTS) * 1000; //Needs to be fixed point...
    //ufixed32_3 tickDex = udiv32_3_lhp(toFixed_3(BUS_CLOCK),(umul32_3_lhp(toFixed_3(track->pitch),umul32_3_lhp(toFixed_3(reload),toFixed_3(ELEMENTS)))));
    //ufixed32_3 tickDex =  track->tickWhole * track->tempo / (4 * 60 * (track->pitch) * ELEMENTS);
    ufixed64_3 tickDex = ((ufixed64_3)track->tickWhole);
    tickDex *= (track->tempo);
    tickDex /= (4 * 60 * (track->pitch) * ELEMENTS);
    track->tickIndex = (ufixed32_3)tickDex;
  }
  else
  {
    //This updates the tick and output voltage
    //track->tickCount++;
    track->tickCount += 1000; //Increment the tick count
    track->tickICount += 1000;
    if( (track->tickICount) >= (track->tickIndex))
    {
      //If it takes less than one tick to increment the table index, divide and add
      if(track->tickIndex < 1000)
      {
        //track->tableIndex += toDec_3((1000 / track->tickIndex));
        track->tableIndex += (1000 / track->tickIndex);
        track->tickICount = 0;
      }
      else
      {
        track->tableIndex += 1;
        track->tickICount -= track->tickIndex;
      }
      /*
      if(track->tableIndex >= ELEMENTS)
      {
        track->tableIndex = 0;
      }
      */
      track->tableIndex = track->tableIndex % ELEMENTS;
    }
    track->output = (instruments[track->instrument])[track->tableIndex];  //get the output voltage for that instrument
    track->output = (track->output * track->volume) / 15; //Amplitude based of volume
  }
}


//---BMML_ProcessTrack---
//Inputs : Pointer to a BMML_Track struct, reload value for timer
//Outputs: None
//Will update the PC of the track to the newest NOTE, the only time dependent element. E.g. it will process all the tempo changes and length changes before setting the next note
void BMML_ParseProgram(struct BMML_Track* track, uint32_t reload)
{
  uint8_t note = 0;     //Has a note been encountered?
  uint8_t current;      //Current item pointed to by the PC
  uint8_t pitchIndex;   //Index of pitch
  uint8_t operand;          //temporary space to deal with operands, be it out of range or to calculate tickTotal
  while(!note)
  {
    current = track->program[track->pc];
    if(!current)
    {
      return; //if current is null, string terminated
    }
    track->pc++;  //If a case uses this incremented pc to access its operand, it must keep be responsible to keep the pc updated to the next instruction
    switch (current)
    {
      case 'o':
        operand = track->program[track->pc];
        if(operand>8){operand = 8;}
        track->octave = operand;  //PC already sits on the operand. Increment pc after. Max 8
        track->pc++;
        break;
      case 'v':
        operand = track->program[track->pc];
        if(operand>15){operand = 15;}
        track->volume = operand;  //PC already sits on the operand. Increment pc after. Max 15
        track->pc++;
        break;
      case 't':
        track->tempo = track->program[track->pc];         //PC already sits on the operand. Increment pc after. Max 255. Heh 8 bit max is 255 ;P
        track->pc++;
        track->tickWhole = BMML_CalcWhole(BUS_CLOCK,track->tempo,reload); 
        break;
      case 'l':
        operand = track->program[track->pc];              //PC already sits on the operand. Increment pc after. Max 64
        track->pc++;
        if(operand == 1 || operand == 2 || operand == 4 || operand == 8 || operand == 12 || operand == 16 || operand == 24 || operand == 32 || operand == 64)
        {
          track->length = operand;
        }
        break;
      case 'i':
        track->instrument = track->program[track->pc];
        track->pc++;
        break;
      case '<':
        track->octave--;
        break;
      case '>':
        track->octave++;
        break;
      case 'j':
        operand = track->program[track->pc]; //pc already incremented
        if(track->counter!=0)
        {
          if(operand == '+')
          {
            track->pc++;
            track->pc += track->program[track->pc];
          }
          else if(operand == '-')
          {
            track->pc++;
            track->pc -= track->program[track->pc];
          }
          else
          {
            track->pc = track->program[track->pc];
          }
          track->pc++;  //update pc to next instruction
        }
        else
        {
          if(operand == '+')
          {
            track->pc++;  //Skip this optional operand
          }
          else if(operand == '-')
          {
            track->pc++;
          }
          else
          {
          }
          track->pc++;  //update pc to next instruction
        }
        break;
      case 'k':
        operand = track->program[track->pc]; //pc already incremented
        if(operand == '+')
        {
          track->pc++;
          track->counter += track->program[track->pc];
        }
        else if(operand == '-')
        {
          track->pc++;
          track->counter -= track->program[track->pc];
        }
        else
        {
          track->counter = track->program[track->pc];
        }
        track->pc++;  //increment pc to next instruction
        break;
      default:
        if((current >= 'a' && current <= 'g')|| current == 'r' || current == 'n')
        {
          note = 1; // Note encountered.
          track->tickCount = 0; //set the tickCount to 0
          ////Pitch Calculation
          if(current == 'r')
          {
            track->pitch = 0;
            operand = track->program[track->pc];  //pc already incremented
          }
          else if (current == 'n')
          {
            operand = track->program[track->pc];
            pitchIndex = operand;
            track->pc++;
            operand = track->program[track->pc];

          }
          else
          {
            //Get index of base pitch from table
            switch(current)
            {
              case 'c':
                pitchIndex = track->octave * 12 + 0;
                break;
              case 'd':
                pitchIndex = track->octave * 12 + 2;
                break;
              case 'e':
                pitchIndex = track->octave * 12 + 4;
                break;
              case 'f':
                pitchIndex = track->octave * 12 + 5;
                break;
              case 'g':
                pitchIndex = track->octave * 12 + 7;
                break;
              case 'a':
                pitchIndex = track->octave * 12 + 9;
                break;
              case 'b':
                pitchIndex = track->octave * 12 + 11;
                break;
            }
            
            //Sharp and flat calculation
            operand = track->program[track->pc];  //operand is the next operand
            while(operand == '+' || operand == '-')
            {
              if(operand == '+')
              {
                pitchIndex++;
              }
              else
              {
                pitchIndex--;
              }
              track->pc++;
              operand = track->program[track->pc];
            }
            track->pitch = pitchTable[pitchIndex];
            ////
            //operand already updated for next part
          }
          ////////
          
          
          //Calculate tickTotal.
          //Operand has already been taken
          track->tickTotal = 0; //tick total is currently 0
          
          //Initial calculation
          if(operand == 1 || operand == 2 || operand == 4 || operand == 8 || operand == 12 || operand == 16 || operand == 24 || operand == 32 || operand == 64)
          {
            if(track->program[(track->pc)+1] != '&' && track->program[(track->pc)+1] != '.' ) //if it fulfills these then it should proceed to the loop instead
            {
              track->tickTotal = track->tickWhole/operand;
              track->pc++;  //update dat PC
              break;
            }
          }
          else if (operand=='.') //if next is going to be a dot, add the predot amount
          {
            //track->tickTotal = udiv32_3((track->tickWhole),toFixed_3((track->length)));
            track->tickTotal = track->tickWhole/track->length;
          }
          else if (operand=='&') //if it's already on an &, add the tick now
          {
            track->tickTotal = track->tickWhole/track->length;
          }
          else
          {
            if(track->program[(track->pc)+1] != '&' && track->program[(track->pc)+1] != '.' ) //if it fulfills these then it should proceed to the loop instead
            {
              track->tickTotal = track->tickWhole/track->length;
              //no need to update the PC; no compatible operands
              break;
            }
          }
          
          ////Repeated calculation
          ////This is by no way the most elegant way of doing it. I would do it by recursion, but I just want it to be working
          uint8_t length = track->length; //length of the last length. Basically only for the dotted note to use
          //each case is responsible for breaking out
          while(1)
          {
            if(operand == 1 || operand == 2 || operand == 4 || operand == 8 || operand == 12 || operand == 16 || operand == 24 || operand == 32 || operand == 64)
            {
              track->tickTotal += track->tickWhole/operand;
              length = operand; //update the length in case of dots
              track->pc++;  //update the PC
              if(track->program[track->pc] == current)
              {
                break;
              }
            }
            else if(operand == '.')
            {
              track->tickTotal += track->tickWhole/(length*2);
              track->pc++;
              if(track->program[track->pc] == current)
              {
                break;
              }
            }
            else if(operand == '&')
            {
              track->pc++;  //check out the next operand
              if(track->program[track->pc] != current)
              {
                break;
              }
              if(track->program[track->pc + 1] == '-' || track->program[track->pc + 1] == '+') //if it's an altered note, need to skip it
              {
                track->pc++;
              }
            }
            else if(operand == current) //if it's the current note again
            {
              track->pc++;
              operand = track->program[track->pc];
              //look ahead to check for unacceptable operand
              if(operand == 1 || operand == 2 || operand == 4 || operand == 8 || operand == 12 || operand == 16 || operand == 24 || operand == 32 || operand == 64)
              {
                
              }
              else if (operand == '.')
              {
                length = track->length;
              }
              else if (operand == '&')
              {
                length = track->length; //reset the length back to original
                track->tickTotal += track->tickWhole/length;
              }
              else
              {
                track->tickTotal += track->tickWhole/track->length;
                break;
              }
            }
            else
            {
              break;
            }
            operand = track->program[track->pc];
          }
          ////////
          
        }
        break;
    }
  }
}
