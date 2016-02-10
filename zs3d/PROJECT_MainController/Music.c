#include "Music.h"
#include "../../drivers/UART.h"

//Music Controller Command List
//Commands
#define WAIT        0x00    //Waits for a STX
#define STX         0x02     //Transmission started. Hunts for a command
#define EDX         0x03     //Transmission end. Will go to WAIT
#define M_RESET     0x10
#define LOAD_SONG   0x11
#define PAUSE_SONG  0x12
#define PLAY_SONG   0x13
#define PLAY_SOUND  0x14

void Music_Init(void)
{
  UART2_Init(BAUD_115200);
}

void Music_Reset(void)
{
  UART2_OutChar(0x02);
  UART2_OutChar(M_RESET);
  UART2_OutChar(0x03);
}
void Music_LoadSong(uint8_t songIndex)
{
  UART2_OutChar(0x02);
  UART2_OutChar(LOAD_SONG);
  UART2_OutChar(songIndex);
  UART2_OutChar(0x03);
}
void Music_PlaySong(void)
{
  UART2_OutChar(0x02);
  UART2_OutChar(PLAY_SONG);
  UART2_OutChar(0x03);
}
void Music_PauseSong(void)
{
  UART2_OutChar(0x02);
  UART2_OutChar(PAUSE_SONG);
  UART2_OutChar(0x03);
}
void Music_PlaySound(uint8_t soundIndex)
{
  UART2_OutChar(0x02);
  UART2_OutChar(PLAY_SOUND);
  UART2_OutChar(soundIndex);
  UART2_OutChar(0x03);
}
