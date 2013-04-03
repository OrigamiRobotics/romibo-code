// RomiboSound.cpp : wrapper code for the WaveHC library, customized for the Romibo robot
// Copyright (c) 2011 Aubrey Shick, Brian Pfiffner, Garth Zeglin
// ##BEGIN_LICENSE##
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this file; if not, write to the Free Software Foundation,
// Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
// ##END_LICENSE##
/****************************************************************/

#include <avr/pgmspace.h>

// Include the WaveHC library for reading from the SD card and playing sounds
#include "WaveHC.h"
#include "FatReader.h"
#include "SdReader.h"
#include "WaveUtil.h"

#include "RomiboSound.h"

// SD Card variables
static SdReader card;    // This object holds the information for the card
static FatVolume vol;    // This holds the information for the partition on the card
static FatReader root;   // This holds the information for the filesystem on the card
static FatReader f;      // This holds the information for the file we're playing

static uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
static dir_t dirBuf;     // buffer for directory reads

// Sound Interface.  For the moment, these are still global, but should private.
static WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
static boolean sound;    // This variable is true if the sound has successfully been initiated
static int failin;       // Sound initialization error code number

int sdSetup(void)
{
  sound = true;
  failin = 0;

  // Initialize the SD Card for sound usage
  if (!card.init()) {
    #ifdef DEBUG
      putstring_nl("Card init. failed");
      sdErrorCheck();
    #endif
    sound = false;
    failin = 1;
    return 1;
  }
  
  // Enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
  
  // Look for a FAT partition on the SD Card
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // found one
  }
  
  // If a valid FAT partition was not found
  if (part == 5) {
    #ifdef DEBUG
      putstring_nl("No valid FAT partition");
      sdErrorCheck();
    #endif
    sound = false;
    failin = 2;
    return 2;
  }

#ifdef DEBUG
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
#endif

  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    sound = false;
    failin = 3;
    return 3;
  }
  dirLevel = 0;

  return 0;
}

void sdErrorCheck(void){
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  sound = false;
  failin = 4;
}

void playfile(char* name){
  
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
  
}
boolean soundEnabled(void)
{
  return sound;
}
boolean soundIsPlaying(void)
{
  return wave.isplaying;
}
void stopSound(void)
{
  wave.stop();
}
int lastSoundError(void)
{
  return failin;
}
